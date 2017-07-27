
external isUrl : string => bool = "is-url" [@@bs.module];

let space = 32.0;
let newline = 10.0;

/* Converts raw text to text elements with embeded markup like
   links or images. */

let parseToWords text => {
  let chars = Js.String.split "" text;
  let words = [||];
  let word = ref "";
  Js.Array.forEach (fun char => {
    let codePoint = Js.String.charCodeAt 0 char;
    if (codePoint == space || codePoint == newline) {
      if (!word != "") {
        ignore @@ Js.Array.push !word words;
        word := "";
      };
      if (codePoint == newline) {
        ignore @@ Js.Array.push char words;
      };
    } else {
      word := !word ^ char;
    }
  }) chars;
  if (!word != "") {
    ignore @@ Js.Array.push !word words;
  };
  words
};

let textToMarkup text => {
  let words = parseToWords text;
  let lastWordIndex = (Js.Array.length words) - 1;
  Js.Array.mapi (fun word i => {
    if (isUrl word) {
      <a href=word target="_black" rel="noreferrer">(ReasonReact.stringToElement word)</a>
    } else {
      let word = lastWordIndex == i ? word : (word ^ " ");
      ReasonReact.stringToElement (word)
    }
  }) words
};