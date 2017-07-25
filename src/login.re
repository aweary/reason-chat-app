let component = ReasonReact.statelessComponent "Login";

let make _children => {
  ...component,
  render: fun _ => {
    <div>
      <h1>(ReasonReact.stringToElement "Enter a username")</h1>
      <input />
    </div>
  }
}