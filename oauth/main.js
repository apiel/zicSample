const baseHeader = {
    // 'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
    // 'Accept-Encoding': 'gzip, deflate, br',
    // 'Accept-Language': 'en-US,en;q=0.9',
    // 'Connection': 'keep-alive',
    // 'Host': 'freesound.org',
    // 'Sec-Fetch-Dest': 'document',
    // 'Sec-Fetch-Mode': 'navigate',
    // 'Sec-Fetch-Site': 'none',
    // 'Sec-Fetch-User': '?1',
    // 'Upgrade-Insecure-Requests': '1',
    // 'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36',
    // 'sec-ch-ua': '"Chromium";v="112", "Google Chrome";v="112", "Not:A-Brand";v="99"',
    // 'sec-ch-ua-mobile': '?0',
    // 'sec-ch-ua-platform': '"Linux"',
};

async function main() {
    const username = process.argv[2];
    const password = process.argv[3];

    const res = await fetch(
        'https://freesound.org/apiv2/oauth2/authorize/?client_id=aOE7UaYW68l205T8RHuH&response_type=code',
        {
            headers: {
                ...baseHeader,
            }
        }
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
    // data.append('next', '/apiv2/oauth2/authorize/?client_id=aOE7UaYW68l205T8RHuH&response_type=code');
    data.append('username', username);
    data.append('password', password);
    const postParams = {
        method: 'POST',
        body: data,
        // redirect: 'manual',
        // referrer: res.url,
        // origin: 'https://freesound.org',
        // coookies: res.headers.get('Set-Cookie'),
        headers: {
            ...baseHeader,
            'Content-Type': 'application/x-www-form-urlencoded',
            Referer: res.url,
            // 'Set-Cookie': res.headers.get('Set-Cookie'),
            // 'Cookie': res.headers.get('Set-Cookie'),
            Cookie: cookie,
            // Origin: 'https://freesound.org',
            // Host: 'freesound.org',
        },
    };
    const [postUrl] = res.url.split('?');
    const res2 = await fetch(postUrl, postParams);
    // const res2 = await fetch(res.url, postParams);
    const text2 = await res2.text();

    console.log({
        postUrl,
        postParams,
        body: text2.match(/<body[^>]*>([\w|\W]*)<\/body>/im)?.[0],
        status: res2.status,
        statusText: res2.statusText,
        loc: res2.headers.get('Location'),
        url: res2.url,
    });
}

main();
