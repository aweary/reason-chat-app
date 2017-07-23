external require : string => unit = "" [@@bs.val];

require "../../../src/styles.css";

ReactDOMRe.renderToElementWithId <Root /> "app";