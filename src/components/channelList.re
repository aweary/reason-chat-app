let component = ReasonReact.statelessComponent "ChannelList";

let make _ => {
  ...component,
  render: fun self => {
    <ul className="channel-list">
      <li className="channel-list--item channel-active-item">(ReasonReact.stringToElement "# general")</li>
      <li className="channel-list--item">(ReasonReact.stringToElement "# reasonml")</li>
      <li className="channel-list--item">(ReasonReact.stringToElement "# react")</li>
    </ul>
  }
}