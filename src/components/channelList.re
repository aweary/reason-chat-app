open Types;
let component = ReasonReact.statelessComponent "ChannelList";

let mapChannelsToElements channels active onChannelSelect => {
  Js.Array.map (fun (channel: channel) => {
    let className = channel.id == active ? "channel-list--item channel-active-item" : "channel-list--item";
    <li key=(string_of_int channel.id) className onClick=(fun _ => onChannelSelect(channel.id))>
      (ReasonReact.stringToElement ("# " ^ channel.name))
    </li>
  }) channels
};

let make ::active ::channels ::onChannelSelect _ => {
  ...component,
  render: fun self => {
    <ul className="channel-list">
      (ReasonReact.arrayToElement (mapChannelsToElements channels active onChannelSelect))
    </ul>
  }
}