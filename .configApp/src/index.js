import {Component} from 'preact';

import {ConfigForm} from "./ConfigForm";

import {css} from 'emotion';
import wretch from "wretch";



const configValidator = (data) => {
    console.log('validating');
    let errors = [];
    let warnings = [];
    if (!data.wifi_ssid) errors.push({type: 'wifi_ssid', message: 'Wifi SSID cannot be empty'})
    if (!data.wifi_password) errors.push({type: 'wifi_password', message: 'Wifi Password cannot be empty'})
    if (data.api_host) {
        if (data.api_host.indexOf('http://')) data.api_host = data.api_host.replace('http://','');
        if (data.api_host.indexOf('https://')) data.api_host = data.api_host.replace('https://','');
        if (data.api_host.charAt(data.api_host.length-1) === '/') data.api_host = data.api_host.slice(0, -1);
        if ('!@#$%^&*():/\\,;[]{}=+~`\'"|'.split('').some((char) => data.api_host.indexOf(char) > 1))
            errors.push({type: 'api_host', message: 'Invalid character in API Host'});
        if (data.api_host.indexOf('.') < 1) errors.push({type: 'api_host', message: 'API Host should be in the form of "api.northpoler.com"'})
   } else {
        warnings.push({type: 'api_host', message: 'Using api-staging.northpoler.com'});
        data.api_host = 'api-staging.northpoler.com';
    }
    return {data, errors, warnings};
}

const api = {
    config: {
        get: () => new Promise((resolve, reject) => {
            wretch('http://localhost:3000/api/config')
                .get()
                .json(data => {
                    console.log('success!')
                    console.log(data);
                    resolve(data);
                })
        }),
        set: (data) => new Promise((resolve, reject) => {
            console.log(data);
            wretch('http://localhost:3000/api/config')
                .post(data)
                .res(response => {
                    console.log('🎉')
                    console.log(response);
                    resolve(data);
                })
                .catch(err => {
                    console.log('😢')
                    console.log(err);
                    reject(err);
                })
        }),
    }
}
export default class App extends Component {
    state = {
        boot_to_config: false,
        wifi_ssid: null,
        wifi_password: null,
        api_host: null,
    }

    componentDidMount() {
        api
            .config
            .get()
            .then((data) => { this.setState(data) });
    }

    inputHandler = e => {
        e.preventDefault();
        const {name, value} = e.target;
        const _state = this.state;
        _state[name] = value;
        this.setState(_state);
    };
    bootToConfigSwitchHandler = e => {
        e.preventDefault();
        const {value} = e.target;
        const _state = { ...this.state, boot_to_config: value === 'on'};
        this.setState(_state);
        e.target.blur();
    };
    submitHandler = (e, setErrors, setWarnings) => {
        e.preventDefault();
        console.log('submitting!');
        const {data, errors, warnings} = configValidator(this.state);
        if (warnings.length) setWarnings(warnings);
        if (errors.length) {
            setErrors(errors);
            return false;
        } else {
            this.setState(data);
            api
                .config
                .set(data)
                .then(data => console.log('🎉'))
                .catch(err => console.log('😢'))
        }
    }

    render() {
        return (
            <div className={css`
				font-family: Helvetica, Arial, sans-serif;
				display: flex;
				justify-content: center;
				align-content: center;
			`}>
                <ConfigForm
                    configUrl={this.state.configUrl}
                    conf={{
                        boot_to_config: this.state.boot_to_config,
                        wifi_ssid: this.state.wifi_ssid,
                        wifi_password: this.state.wifi_password,
                        api_host: this.state.api_host,
                    }}
                    toggleSwitchHandler={this.toggleSwitchHandler}
                    inputHandler={this.inputHandler}
                    bootToConfigOptions={[
                        {name: 'On', active: this.state.boot_to_config},
                        {name: 'Off', active: !this.state.boot_to_config}
                    ]}
                    bootToConfigSwitchHandler={this.bootToConfigSwitchHandler}
                    submitHandler={this.submitHandler}
                    css={css`width: 400px`}
                />
            </div>
        );
    }
}
