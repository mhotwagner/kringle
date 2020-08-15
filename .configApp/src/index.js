import {Component} from 'preact';

import {css} from 'emotion';
import {ConfigForm} from "./ConfigForm";


export default class App extends Component {
    state = {
        configUrl: "/config.html",
        boot_to_config: false,
        wifi_ssid: null,
        wifi_password: null,
        api_host: null,
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

        render() {
        return (
            <div className={css`
				font-family: Helvetica, sans-serif;
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
                />
            </div>
        );
    }
}
