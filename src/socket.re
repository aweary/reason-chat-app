open Types;

type socket;
external createClient : string => socket = "socket.io-client" [@@bs.module];
external emit : socket => string => string => unit = "emit" [@@bs.send];
let socketAddress = "http://localhost:3000";
let userEvent = "@user";
let messageEvent = "@message";

let emitNewUser socket (user: user) => {
    switch (Js_json.stringifyAny { "id": user.id, "nickname": user.nickname }) {
    | Some stringifiedUser => emit socket userEvent stringifiedUser
    | None => ()
    };
};