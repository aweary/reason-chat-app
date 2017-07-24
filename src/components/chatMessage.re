open Types;

let component = ReasonReact.statelessComponent "ChatMessage";

let make ::message _ => {
  ...component,
  render: fun _ => {
    <li className="chat-message">
      <img className="user-avatar" src=message.author.picture />
      <div className="chat-message--content">
      <div>
      <span className="chat-message--user">(ReasonReact.stringToElement message.author.nickname)</span>
      <span className="chat-message--time">(ReasonReact.stringToElement "Today at 12:30")</span>
      </div>
       <div className="chat-message--text">
       (ReasonReact.stringToElement message.content)
       </div>
      </div>
    </li>
  }
}