let component = ReasonReact.statelessComponent "Content";

let make ::onNewMessage ::messages ::users ::channel _ => {
  {
  ...component,
  render: fun _ => {
    <div className="chat-content">
      <MessageList messages users />
      <MessageInput channel onNewMessage />
    </div>
  }
}
}