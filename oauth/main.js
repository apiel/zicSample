async function getBody(res) {
    const text = await res.text();
    return text.match(/<body[^>]*>([\w|\W]*)<\/body>/im)?.[0];
}

function getInputValue(body, name) {
    const csrfSearch = `name="${name}" value="`;
    const csrfIndex = body.indexOf(csrfSearch);
    const csrf = body.substring(
        csrfIndex + csrfSearch.length,
        body.substring(csrfIndex + csrfSearch.length).indexOf('"') + csrfIndex + csrfSearch.length
    );
    return csrf;
}

function getSetCookies(res) {
    const cookies = [];
    for (const [key, value] of res.headers) {
        if (key === 'set-cookie') {
            cookies.push(value.split(';')[0]);
        }
    }
    return cookies;
}

function getCookies(res) {
    return  getSetCookies(res).join('; ');
}

async function main() {
    const username = process.argv[2];
    const password = process.argv[3];

    const res1 = await fetch(
        'https://freesound.org/apiv2/oauth2/authorize/?client_id=aOE7UaYW68l205T8RHuH&response_type=code',
        {
            headers: {
                accept: 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
                'accept-language': 'en-US,en;q=0.9',
                'sec-ch-ua': '"Chromium";v="112", "Google Chrome";v="112", "Not:A-Brand";v="99"',
                'sec-ch-ua-mobile': '?0',
                'sec-ch-ua-platform': '"Linux"',
                'sec-fetch-dest': 'document',
                'sec-fetch-mode': 'navigate',
                'sec-fetch-site': 'none',
                'sec-fetch-user': '?1',
                'upgrade-insecure-requests': '1',
            },
            referrerPolicy: 'strict-origin-when-cross-origin',
            body: null,
            method: 'GET',
        }
    );
    const body1 = await getBody(res1);
    console.log(`res1 (${res1.status}: ${res1.url}):\n`, body1);
    console.log(`res1 set-cookie:`, getSetCookies(res1));

    const csrf = getInputValue(body1, 'csrfmiddlewaretoken');
    console.log(`\n\ncsrf: ${csrf}`);
    const cookie = getCookies(res1);
    console.log(`cookie: ${cookie}`);
    const res3 = await fetch('https://freesound.org/apiv2/login/', {
        headers: {
            accept: 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
            'accept-language': 'en-US,en;q=0.9',
            'cache-control': 'max-age=0',
            'content-type': 'application/x-www-form-urlencoded',
            'sec-ch-ua': '"Chromium";v="112", "Google Chrome";v="112", "Not:A-Brand";v="99"',
            'sec-ch-ua-mobile': '?0',
            'sec-ch-ua-platform': '"Linux"',
            'sec-fetch-dest': 'document',
            'sec-fetch-mode': 'navigate',
            'sec-fetch-site': 'same-origin',
            'sec-fetch-user': '?1',
            'upgrade-insecure-requests': '1',
            cookie,
            Referer:
                'https://freesound.org/apiv2/login/?next=/apiv2/oauth2/authorize/%3Fclient_id%3DaOE7UaYW68l205T8RHuH%26response_type%3Dcode',
            'Referrer-Policy': 'no-referrer-when-downgrade',
        },
        body: `csrfmiddlewaretoken=${csrf}&username=${username}&password=${password}&next=%2Fapiv2%2Foauth2%2Fauthorize%2F%3Fclient_id%3DaOE7UaYW68l205T8RHuH%26response_type%3Dcode`,
        method: 'POST',
    });
    console.log(`res3 (${res3.status}: ${res3.url}):\n`, await getBody(res3));
    console.log(`res3 set-cookie:`, getSetCookies(res3));
}
main();
