open ReasonJs;

let storageKey = "@rc_id";

let readTokenFromStorage () => {
  switch (Storage.getItem storageKey Storage.localStorage) {
  | Some token => token
  | None => ""
  }
};

let writeTokenToStorage token => {
  Storage.setItem storageKey token Storage.localStorage
}