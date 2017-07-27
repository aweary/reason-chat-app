let component = ReasonReact.statelessComponent "MentionList";

let make ::show _children => {
  ...component,
  render: fun _ =>
    if (not show) {
      <div />
    } else {
      <div className="mention-list" />
    }
};