let component = ReasonReact.statelessComponent "MentionList";

let make ::show _children => {
  ...component,
  render: fun _ =>
    if (show) {
      <div className="mention-list" />
    } else {
      ReasonReact.nullElement
    }
};