type state = {
  value: string
};

let component = ReasonReact.statefulComponent "Login";

let make ::onJoinAttempt ::joinError _children => {
  let onJoinButtonClick _ {ReasonReact.state: state} => {
    onJoinAttempt state.value;
    ReasonReact.NoUpdate
  };
  let handleInputChange event {ReasonReact.state: state} => {
    let target = event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
    ReasonReact.Update {...state, value: target##value }
  };
  {
    ...component,
    initialState: fun _ => { value: "" },
    render: fun self => {
      let error = switch (joinError) {
      | Some err => <span className="join-error">(ReasonReact.stringToElement err)</span>
      | None => <span />
      };
      <div className="login-page">
        <h1> (ReasonReact.stringToElement "Enter a username") </h1>
        error
        <input value=(self.state.value) onChange=(self.update handleInputChange) />
        <button className="login-button" onClick=(self.update onJoinButtonClick)>
          (ReasonReact.stringToElement "Join")
        </button>
      </div>
  }
  }
};