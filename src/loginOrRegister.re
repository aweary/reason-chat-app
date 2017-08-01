module LoginStyles = {
  let container =
    Styles.(
      compose [|
        flex.centered,
        flex.column,
        css {"height": height.full, "color": colors.primary, "maxWidth": 300}
      |]
    );
  let button = Styles.(compose [|primaryButton, css {"width": forms.elementWidth}|]);
};

let str = ReasonReact.stringToElement;

type input =
  | Username
  | Password
  | ConfirmPassword;

type page =
  | Login
  | SignUp;

type state = {
  page,
  username: string,
  password: string,
  confirmPassword: string
};

let component = ReasonReact.statefulComponent "LoginOrRegister";

let togglePageType _ {ReasonReact.state: state} =>
  ReasonReact.Update {
    ...state,
    password: "",
    confirmPassword: "",
    page:
      switch state.page {
      | SignUp => Login
      | Login => SignUp
      }
  };

let handleInputChange input event {ReasonReact.state: state} => {
  let target = event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
  switch input {
  | Username => ReasonReact.Update {...state, username: target##value}
  | Password => ReasonReact.Update {...state, password: target##value}
  | ConfirmPassword => ReasonReact.Update {...state, confirmPassword: target##value}
  }
};

let make ::onSignUp ::onLogin _children => {
  let onButtonClick _event {ReasonReact.state: state} => {
    let handler = switch state.page {
    | SignUp => onSignUp
    | Login => onLogin
    };
    handler state.username state.password;
    ReasonReact.NoUpdate
  };
  {
  ...component,
  initialState: fun _self => {page: SignUp, username: "", password: "", confirmPassword: ""},
  render: fun self => {
    let confirmPassword =
      switch self.state.page {
      | SignUp =>
        <input
          onChange=(self.update (handleInputChange ConfirmPassword))
          value=self.state.confirmPassword
          className=Styles.input
          placeholder="Confirm password..."
          _type="password"
        />
      | _ => ReasonReact.nullElement
      };
    let title =
      switch self.state.page {
      | SignUp => "Welcome aboard."
      | Login => "Welcome back."
      };
    let linkText =
      switch self.state.page {
      | SignUp => "Already have an account? Login here."
      | Login => "Don't have an account? Sign up here."
      };
    let invalidInput =
      Js.Boolean.to_js_boolean (
        not (
          switch self.state.page {
          | SignUp =>
            self.state.username != "" &&
            self.state.password != "" && self.state.password == self.state.confirmPassword
          | Login => self.state.username != "" && self.state.password != ""
          }
        )
      );
    let buttonText =
      switch self.state.page {
      | SignUp => "Sign Up"
      | Login => "Login"
      };
    <div className=LoginStyles.container>
      <h1 className="bar"> (str title) </h1>
      <input
        onChange=(self.update (handleInputChange Username))
        value=self.state.username
        className=Styles.input
        placeholder="Username..."
      />
      <input
        onChange=(self.update (handleInputChange Password))
        value=self.state.password
        className=Styles.input
        placeholder="Password..."
        _type="password"
      />
      confirmPassword
      <button onClick=(self.update onButtonClick) disabled=invalidInput className=LoginStyles.button>
        (str buttonText)
      </button>
      <a href="#" onClick=(self.update togglePageType)> (str linkText) </a>
    </div>
  }
}
}