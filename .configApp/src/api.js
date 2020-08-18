import wretch from "wretch";

const API_HOST = '192.168.1.1'

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
            console.log(data);
            wretch(`http://${API_HOST}/api/config`)
                .post(data)
                .res(response => {
                    console.log('🎉');
                    console.log(response);
                    resolve(data);
                })
                .catch(err => {
                    console.log('😢');
                    console.log(err);
                    reject(err);
                })
        }),
    }
}
