async function main() {
    const username = process.argv[2];
    const password = process.argv[3];

    const res = await fetch(
        'https://freesound.org/apiv2/oauth2/authorize/?client_id=aOE7UaYW68l205T8RHuH&response_type=code'
    );
    const text = await res.text();

    var inputs = text.match(/<input.+?\/?>/g);
    console.log(inputs);
    const csrfSearch = 'name="csrfmiddlewaretoken" value="';
    const csrfIndex = text.indexOf(csrfSearch);
    const csrf = text.substring(
        csrfIndex + csrfSearch.length,
        text.substring(csrfIndex + csrfSearch.length).indexOf('"') + csrfIndex + csrfSearch.length
    );

    const nextSearch = 'name="next" value="';
    const nextIndex = text.indexOf(nextSearch);
    const next = text.substring(
        nextIndex + nextSearch.length,
        text.substring(nextIndex + nextSearch.length).indexOf('"') + nextIndex + nextSearch.length
    );

    const [cookie] = res.headers.get('Set-Cookie').split(';');
    console.log({
        next,
        csrf,
        username,
        password,
        referer: res.url,
        setCookie: res.headers.get('Set-Cookie'),
        cookie,
    });

    const data = new URLSearchParams();
    data.append('csrfmiddlewaretoken', csrf);
    data.append('next', next);
    data.append('username', username);
    data.append('password', password);
    const res2 = await fetch(res.url, {
        method: 'POST',
        body: data,
        // redirect: 'manual',
        // referrer: res.url,
        // origin: 'https://freesound.org',
        // coookies: res.headers.get('Set-Cookie'),
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
            Referer: res.url,
            // 'Set-Cookie': res.headers.get('Set-Cookie'),
            // 'Cookie': res.headers.get('Set-Cookie'),
            Cookie: cookie,
            // Origin: 'https://freesound.org',
            // Host: 'freesound.org',
        },
    });
    const text2 = await res2.text();

    console.log(text2, res2.status, res2.statusText, res2.headers.get('location'));
}

main();
