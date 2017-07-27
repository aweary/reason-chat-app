open Types;

let component = ReasonReact.statelessComponent "Toolbar";

let make ::channel _children => {
  ...component,
  render: fun _ => {
    let placeholder =
      switch channel {
      | Some c => "#" ^ c.name
      | None => ""
      };
    let topic =
      switch channel {
      | Some c => c.topic
      | None => ""
      };
    <div className="toolbar">
      <span className="toolbar-title"> (ReasonReact.stringToElement placeholder) </span>
      <span className="toolbar-topic"> (ReasonReact.stringToElement topic) </span>
    </div>
  }
};