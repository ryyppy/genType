// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE


function create(x) {
  return {
          contents: x
        };
}

function access(r) {
  return r.contents + 1 | 0;
}

function update(r) {
  r.contents = r.contents + 1 | 0;
  return /* () */0;
}

function get(r) {
  return r.contents;
}

function make(prim) {
  return {
          contents: prim
        };
}

function set(r, v) {
  r.contents = v;
  return /* () */0;
}

var R = {
  get: get,
  make: make,
  set: set
};

function destroysRefIdentity(x) {
  return x;
}

function preserveRefIdentity(x) {
  return x;
}

export {
  create ,
  access ,
  update ,
  R ,
  get ,
  make ,
  set ,
  destroysRefIdentity ,
  preserveRefIdentity ,
  
}
/* No side effect */
