import wretch from "wretch";

const API_HOST = '192.168.1.1';

export const api = {
    config: {
        get: () => new Promise((resolve, reject) => {
            wretch(`http://${API_HOST}/api/config`)
                .get()
                .json(data => {
                    console.log('success!');
                    console.log(data);
                    resolve(data);
                })
        }),
        set: (data) => new Promise((resolve, reject) => {
            console.log('api.config.set');
            console.log(data);
            console.log(JSON.stringify(data));
            wretch(`http://${API_HOST}/api/config`)
                .options({mode: "no-cors"})
                .post(data)
                .res(response => {
                    console.log('🎉');
                    console.log(response);
                    resolve(response);
                })
                .catch(err => {
                    console.log('😢');
                    console.log(err);
                    reject(err);
                })
        }),
    }
}
