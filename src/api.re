open Types;

open Bs_fetch;

external apiHost : string = "process.env.API_HOST" [@@bs.val];

external jsonToObject : Js.Json.t => Js.t {..} = "%identity";

let encodeAuthData username password =>
  switch (Js.Json.stringifyAny {"username": username, "password": password}) {
  | Some str => str
  | None => Js.Exn.raiseError "Bad username"
  };

let getAuthenticatedFetchHeaders (token: string) =>
  /* let json = switch (Js.Json.stringifyAny { "token": token }) {
       | Some str => str
       | None => Js.Exn.raiseError "Issue validating message"
     } */
  HeadersInit.make {"x-access-token": token, "Content-Type": "application/json"};

let authenticatedFetch ::method_ ::path ::onSuccess ::onFailure ::token () => {
  let headers = getAuthenticatedFetchHeaders token;
  let requestInit = RequestInit.make ::headers ::method_ ();
  Js.log "Making the request!";
  ignore @@
  Js.Promise.(
    fetchWithInit (apiHost ^ path) requestInit |>
    then_ (
      fun response => {
        let status = Response.status response;
        let json = Response.json response;
        then_
          (
            fun jsonText =>
              {
                if (status == 200) {
                  onSuccess jsonText
                } else {
                  onFailure jsonText
                };
                Js.log status
              } |> resolve
          )
          json
      }
    )
  )
};

let validateToken ::token ::onSuccess ::onFailure () => {
  Js.log "Calling authenticatedFetch";
  authenticatedFetch method_::Post path::"auth" ::token ::onSuccess ::onFailure ();
};

let authenticate username password => {
  let body = encodeAuthData username password;
  let headers = HeadersInit.make {"Content-Type": "application/json"};
  Js.Promise.(
    fetchWithInit
      (apiHost ^ "auth") (RequestInit.make method_::Post body::(BodyInit.make body) ::headers ()) |>
    then_ Response.text |>
    then_ (fun text => Js.log text |> resolve)
  )
};

let registerUser username password onSuccess onFailure => {
  let body = encodeAuthData username password;
  let headers = HeadersInit.make {"Content-Type": "application/json"};
  Js.Promise.(
    fetchWithInit
      (apiHost ^ "signup") (RequestInit.make method_::Post body::(BodyInit.make body) ::headers ()) |>
    then_ (
      fun response => {
        let status = Response.status response;
        let json = Response.json response;
        then_
          (
            fun jsonText =>
              {
                let responseObj = jsonToObject jsonText;
                if (status == 200) {
                  Js.log "Successful response!";
                  onSuccess responseObj
                } else {
                  Js.log "No dice";
                  onFailure responseObj##error
                };
                Js.log status
              } |> resolve
          )
          json
      }
    )
  )
};

let encodeMessageAuthor message =>
  Json.Encode.(
    object_ [
      ("nickname", string message.author.nickname),
      ("id", string message.author.id),
      ("avatar", string message.author.avatar)
    ]
  );

let encodeMessage2 message =>
  Json.Encode.(
    object_ [
      ("content", string message.content),
      ("id", string message.id),
      ("author", encodeMessageAuthor message),
      ("channel", int message.channel),
      ("timestamp", string message.timestamp)
    ]
  ) |> Js.Json.stringify;

let encodeUser user =>
  Json.Encode.(object_ [("nickname", string user.nickname), ("id", string user.id)]) |> Js.Json.stringify;

let decodeUser userJson =>
  /* @TODO need to send all this info across the wire */
  Json.Decode.{
    firstName: "",
    lastName: "",
    nickname: userJson |> field "nickname" string,
    id: userJson |> field "id" string,
    status: Available,
    picture: "https://semantic-ui.com/images/avatar2/large/matthew.png",
    roles: []
  };

let decodeMessageAuthor messageAuthorJson =>
  Json.Decode.{
    nickname: messageAuthorJson |> field "nickname" string,
    id: messageAuthorJson |> field "id" string,
    avatar: messageAuthorJson |> field "avatar" string
  };

let decodeMessage2 messageJson =>
  Json.Decode.{
    content: messageJson |> field "content" string,
    id: messageJson |> field "id" string,
    author: messageJson |> field "author" decodeMessageAuthor,
    channel: messageJson |> field "channel" int,
    timestamp: messageJson |> field "timestamp" string
  };