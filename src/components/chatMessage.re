open Types;

type tinytime = Js.t {. render [@bs.meth]: Js.Date.t => string };
external tinytime : string => tinytime = "" [@@bs.module];

let component = ReasonReact.statelessComponent "ChatMessage";

let timestampTemplate = tinytime "{h}:{mm}:{s}{a}";

let make ::message _ => {
  ...component,
  shouldUpdate: fun _ => {
    /* TODO eventually messages can update (editing) */
    false
  },
  render: fun _ => {
     let timestamp = timestampTemplate##render (Js.Date.parse message.timestamp); 
    <li className="chat-message">
      <img className="user-avatar" src=message.author.avatar />
      <div className="chat-message--content">
      <div>
      <span className="chat-message--user">(ReasonReact.stringToElement message.author.nickname)</span>
      <span className="chat-message--time">(ReasonReact.stringToElement timestamp)</span>
      </div>
       <div className="chat-message--text">
       (ReasonReact.arrayToElement (TextParser.textToMarkup message.content))
       </div>
      </div>
    </li>
  }
}