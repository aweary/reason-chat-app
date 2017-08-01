type colors = {
  primary: string,
  secondary: string,
  black: string,
  white: string
};

type flex = {
  centered: string,
  column: string
};

type height = {
  full: string
};

type forms = {
  elementWidth: string
};

external css : Js.t {..} => string = "css" [@@bs.module "glamor"];
let compose (classNames: Js.Array.t string) : string => {
  Js.Array.reduce (fun a b => a ^ " " ^ b) "" classNames
};

let flex = {
  centered: css {
    "display": "flex",
    "alignItems": "center",
    "justifyContent": "center",
    "margin": "auto"
  },
  column: css {
    "flexDirection": "column"
  }
};

let colors = {
  primary: "#ff44aa",
  secondary: "#44f",
  black: "#333333",
  white: "#fafafa"
};

let height = {
  full: "100%"
};


let forms = {
  elementWidth: "85%"
};

let input = css {
  "boxSizing": "border-box",
  "color": "#546e7a",
  "backgroundColor": "#e8e8e8",
  "border": "none",
  "borderRadius": "5px",
  "outline": "none",
  "margin-bottom": 20,
  "padding": "20px 10px",
  "width": forms.elementWidth, 
  "resize": "none",
  "height": "28px"
};

let button = css {
  "boxSizing": "border-box",
  "color": colors.white,
  "border": "none",
  "padding": "10px 20px",
  "borderRadius": 5,
  "cursor": "pointer",
  "marginBottom": 20,
  ":disabled": {
    "backgroundColor": "grey",
    "cursor": "not-allowed"
  }
};

let primaryButton = compose [| button, css {
  "backgroundColor": colors.primary
} |];
let secondaryButton = compose [| button, css {
  "backgroundColor": colors.secondary
} |];