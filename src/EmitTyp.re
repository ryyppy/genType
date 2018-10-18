open GenTypeCommon;

let flowExpectedError = "// $FlowExpectedError: Reason checked type sufficiently\n";

let fileHeader = (~language) =>
  switch (language) {
  | Flow => "/** \n * @flow strict\n * @" ++ "generated \n * @nolint\n */\n"
  | Typescript => "/* Typescript file generated by genType. */\n"
  | Untyped => "/* Untyped file generated by genType. */\n"
  };

let outputFileSuffix = (~language) =>
  switch (language) {
  | Flow
  | Untyped => ".re.js"
  | Typescript => ".tsx"
  };

let generatedModuleExtension = (~language) => language == Flow ? ".re" : "";

let shimExtension = (~language) =>
  switch (language) {
  | Flow => ".shim.js"
  | Typescript => ".shim.ts"
  | Untyped => ".shim.not.used"
  };

let genericsString = (~typeVars) =>
  typeVars === [] ? "" : "<" ++ String.concat(",", typeVars) ++ ">";

module Indent = {
  let break = (~indent) =>
    switch (indent) {
    | None => ""
    | Some(s) => "\n" ++ s
    };

  let more = (~indent) =>
    switch (indent) {
    | None => None
    | Some(s) => Some("  " ++ s)
    };

  let heuristic = (~indent, fields) =>
    fields |> List.length > 2 && indent == None ? Some("") : indent;
};

let rec renderTyp = (~language, ~indent=None, typ) =>
  switch (typ) {
  | Array(typ) =>
    let typIsSimple =
      switch (typ) {
      | Ident(_)
      | TypeVar(_) => true
      | _ => false
      };

    if (language == Typescript && typIsSimple) {
      (typ |> renderTyp(~language, ~indent)) ++ "[]";
    } else {
      "Array<" ++ (typ |> renderTyp(~language, ~indent)) ++ ">";
    };

  | Enum({cases, _}) =>
    cases
    |> List.map(case => case.labelJS |> EmitText.quotes)
    |> String.concat(" | ")

  | Function({typeVars, argTypes, retType}) =>
    renderFunType(~language, ~indent, ~typeVars, argTypes, retType)

  | GroupOfLabeledArgs(fields)
  | Object(fields)
  | Record(fields) =>
    let indent1 = fields |> Indent.heuristic(~indent);
    fields |> renderFields(~language, ~indent=indent1);

  | Ident(identPath, typeArguments) =>
    identPath
    ++ genericsString(
         ~typeVars=typeArguments |> List.map(renderTyp(~language, ~indent)),
       )
  | Nullable(typ)
  | Option(typ) =>
    switch (language) {
    | Flow
    | Untyped => "?" ++ (typ |> renderTyp(~language, ~indent))
    | Typescript =>
      "(null | undefined | " ++ (typ |> renderTyp(~language, ~indent)) ++ ")"
    }

  | TypeVar(s) => s
  }
and renderField = (~language, ~indent, (lbl, optness, typ)) => {
  let optMarker = optness === NonMandatory ? "?" : "";
  Indent.break(~indent)
  ++ lbl
  ++ optMarker
  ++ ": "
  ++ (typ |> renderTyp(~language, ~indent));
}
and renderFields = (~language, ~indent, fields) => {
  let indent1 = Indent.more(~indent);
  (language == Flow ? "{|" : "{")
  ++ String.concat(
       ", ",
       List.map(renderField(~language, ~indent=indent1), fields),
     )
  ++ Indent.break(~indent)
  ++ (language == Flow ? "|}" : "}");
}
and renderFunType = (~language, ~indent, ~typeVars, argTypes, retType) =>
  genericsString(~typeVars)
  ++ "("
  ++ String.concat(
       ", ",
       List.mapi(
         (i, t) => {
           let parameterName =
             language == Flow ? "" : "_" ++ string_of_int(i + 1) ++ ":";
           parameterName ++ (t |> renderTyp(~language, ~indent));
         },
         argTypes,
       ),
     )
  ++ ") => "
  ++ (retType |> renderTyp(~language, ~indent));

let typToString = (~language, typ) => typ |> renderTyp(~language);

let ofType = (~language, ~typ, s) =>
  language == Untyped ? s : s ++ ": " ++ (typ |> typToString(~language));

let emitExportConst_ =
    (~early, ~comment="", ~emitters, ~name, ~typ, ~config, line) =>
  (comment == "" ? comment : "// " ++ comment ++ "\n")
  ++ (
    switch (config.module_, config.language) {
    | (_, Typescript)
    | (ES6, _) =>
      "export const "
      ++ (name |> ofType(~language=config.language, ~typ))
      ++ " = "
      ++ line
    | (CommonJS, _) =>
      "const "
      ++ (name |> ofType(~language=config.language, ~typ))
      ++ " = "
      ++ line
      ++ ";\nexports."
      ++ name
      ++ " = "
      ++ name
    }
  )
  |> (early ? Emitters.exportEarly : Emitters.export)(~emitters);

let emitExportConst = emitExportConst_(~early=false);

let emitExportConstEarly = emitExportConst_(~early=true);

let emitExportConstMany = (~emitters, ~name, ~typ, ~config, lines) =>
  lines
  |> String.concat("\n")
  |> emitExportConst(~emitters, ~name, ~typ, ~config);

let emitExportFunction =
    (~early, ~comment="", ~emitters, ~name, ~config, line) =>
  (comment == "" ? comment : "// " ++ comment ++ "\n")
  ++ (
    switch (config.module_, config.language) {
    | (_, Typescript)
    | (ES6, _) => "export function " ++ name ++ line
    | (CommonJS, _) =>
      "function " ++ name ++ line ++ ";\nexports." ++ name ++ " = " ++ name
    }
  )
  |> (early ? Emitters.exportEarly : Emitters.export)(~emitters);

let emitExportDefault = (~emitters, ~config, name) =>
  switch (config.module_, config.language) {
  | (_, Typescript)
  | (ES6, _) =>
    "export default " ++ name ++ ";" |> Emitters.export(~emitters)
  | (CommonJS, _) =>
    "exports.default = " ++ name ++ ";" |> Emitters.export(~emitters)
  };

let emitExportType =
    (~early, ~emitters, ~language, ~opaque, ~resolvedTypeName, ~typeVars, typ) => {
  let export = early ? Emitters.exportEarly : Emitters.export;
  let typeParamsString = genericsString(~typeVars);

  switch (language) {
  | Flow =>
    "export"
    ++ (opaque ? " opaque " : " ")
    ++ "type "
    ++ resolvedTypeName
    ++ typeParamsString
    ++ " = "
    ++ (typ |> renderTyp(~language))
    ++ ";"
    |> export(~emitters)

  | Typescript =>
    if (opaque) {
      /* Represent an opaque type as an absract class with a field called 'opaque'.
         Any type parameters must occur in the type of opaque, so that different
         instantiations are considered different types. */
      let typeOfOpaqueField =
        typeVars == [] ? "any" : typeVars |> String.concat(" | ");
      "// tslint:disable-next-line:max-classes-per-file \n"
      ++ "export abstract class "
      ++ resolvedTypeName
      ++ typeParamsString
      ++ " { protected opaque!: "
      ++ typeOfOpaqueField
      ++ " }; /* simulate opaque types */"
      |> export(~emitters);
    } else {
      "// tslint:disable-next-line:interface-over-type-literal\n"
      ++ "export type "
      ++ resolvedTypeName
      ++ typeParamsString
      ++ " = "
      ++ (typ |> renderTyp(~language))
      ++ ";"
      |> export(~emitters);
    }
  | Untyped => emitters
  };
};

let emitExportVariantType =
    (~emitters, ~language, ~name, ~typeParams, ~variants: list(variant)) =>
  switch (language) {
  | Flow
  | Typescript =>
    "export type "
    ++ name
    ++ genericsString(
         ~typeVars=typeParams |> List.map(typToString(~language)),
       )
    ++ " =\n  | "
    ++ (
      variants
      |> List.map(({name, params}) =>
           Ident(name, params) |> typToString(~language)
         )
      |> String.concat("\n  | ")
    )
    ++ ";"
    |> Emitters.export(~emitters)
  | Untyped => emitters
  };

let emitImportValueAsEarly = (~emitters, ~name, ~nameAs, importPath) =>
  "import "
  ++ (
    switch (nameAs) {
    | Some(nameAs) => "{" ++ name ++ " as " ++ nameAs ++ "}"
    | None => name
    }
  )
  ++ " from"
  ++ "'"
  ++ (importPath |> ImportPath.toString)
  ++ "';"
  |> Emitters.requireEarly(~emitters);

let emitRequire =
    (~early, ~emitters, ~language, ~moduleName, ~strict, importPath) => {
  let commentBeforeRequire =
    switch (language) {
    | Typescript => "// tslint:disable-next-line:no-var-requires\n"
    | Flow => strict ? "" : flowExpectedError
    | Untyped => ""
    };
  commentBeforeRequire
  ++ "const "
  ++ ModuleName.toString(moduleName)
  ++ " = require('"
  ++ (importPath |> ImportPath.toString)
  ++ "');"
  |> (early ? Emitters.requireEarly : Emitters.require)(~emitters);
};

let require = (~early) => early ? Emitters.requireEarly : Emitters.require;

let emitRequireReact = (~early, ~emitters, ~language) =>
  switch (language) {
  | Flow =>
    emitRequire(
      ~early,
      ~emitters,
      ~language,
      ~moduleName=ModuleName.react,
      ~strict=false,
      ImportPath.react,
    )
  | Typescript =>
    "import * as React from 'react';" |> require(~early, ~emitters)
  | Untyped => emitters
  };

let reactComponentType = (~language, ~propsTypeName) =>
  Ident(
    language == Flow ? "React$ComponentType" : "React.ComponentClass",
    [Ident(propsTypeName, [])],
  );

let componentExportName = (~language, ~fileName, ~moduleName) =>
  switch (language) {
  | Flow =>
    fileName == moduleName ? "component" : moduleName |> ModuleName.toString
  | _ => moduleName |> ModuleName.toString
  };

let emitImportTypeAs =
    (~emitters, ~language, ~typeName, ~asTypeName, ~importPath) =>
  switch (language) {
  | Flow
  | Typescript =>
    "import "
    ++ (language == Flow ? "type " : "")
    ++ "{"
    ++ typeName
    ++ (
      switch (asTypeName) {
      | Some(asT) => " as " ++ asT
      | None => ""
      }
    )
    ++ "} from '"
    ++ (importPath |> ImportPath.toString)
    ++ "';"
    |> Emitters.import(~emitters)
  | Untyped => emitters
  };

let blockTagValue = (~language, i) =>
  string_of_int(i) ++ (language == Typescript ? " as any" : "");

let ofTypeAny = (~language, s) => language == Typescript ? s ++ ": any" : s;