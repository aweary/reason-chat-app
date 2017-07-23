let component = ReasonReact.statelessComponent "Content";

let make ::onNewMessage ::messages _ => {
  {
  ...component,
  render: fun _ => {
    <div className="chat-content">
      <MessageList messages />
      <MessageInput onNewMessage />
    </div>
  }
}
}