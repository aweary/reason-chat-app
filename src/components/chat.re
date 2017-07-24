let component = ReasonReact.statelessComponent "Content";

let make ::onNewMessage ::messages ::channel _ => {
  {
  ...component,
  render: fun _ => {
    <div className="chat-content">
      <MessageList messages />
      <MessageInput channel onNewMessage />
    </div>
  }
}
}