import {Component} from 'preact';

import {ConfigForm, configValidator, NOT_SUBMITTED, SUBMIT_SUCCESS, SUBMIT_ERROR} from "./ConfigForm";

import {css} from 'emotion';
import {api} from "./api";

export default class App extends Component {
    state = {
        conf: {
            boot_to_config: false,
            wifi_ssid: null,
            wifi_password: null,
            api_host: null,
        },
        submitting: false,
        submitStatus: NOT_SUBMITTED,
        errors: [],
        warnings: [],
    };

    componentDidMount() {
        api
            .config
            .get()
            .then((data) => { this.setState({...this.state, conf: data}) });
    }

    clearErrorsAndWarnings = name => {
        this.setState({
            conf: this.state.conf,
            submitting: false,
            submitStatus: NOT_SUBMITTED,
            errors: this.state.errors.filter(error => error.type !== name),
            warnings: this.state.warnings.filter(warning => warning.type !== name),
        })
    }
    focusHandler = e => {
        console.log('focusing?');
        const {name} = e.target;
        this.clearErrorsAndWarnings(name);
    }
    inputHandler = e => {
        e.preventDefault();
        const {name, value} = e.target;
        const _conf = this.state.conf;
        _conf[name] = value;
        this.setState({...this.state, conf: _conf});
    };
    bootToConfigSwitchHandler = e => {
        e.preventDefault();
        const {value} = e.target;
        const _conf = this.state.conf;
        _conf.boot_to_config = Number(value === 'on');
        this.setState({ ...this.state, conf: _conf});
        e.target.blur();
    };
    submitHandler = (e) => {
        e.preventDefault();
        console.log('submitting!');
        const {data, errors, warnings} = configValidator(this.state.conf);
        this.setState({...this.state, conf: data});
        if (warnings.length) this.setState({...this.state, warnings});
        if (errors.length) {
            this.setState({...this.state, errors})
            return false;
        } else {
            this.setState({...this.state, submitting: true})
            api
                .config
                .set(this.state.conf)
                .then(data => {
                    console.log('🎉');
                    console.log(data);
                    this.setState({
                        ...this.state,
                        submitting: false,
                        submitStatus: SUBMIT_SUCCESS,
                    });
                })
                .catch(err => {
                    setTimeout(() => {
                        console.log('😢');
                        console.log(err);
                        this.setState({
                            ...this.state,
                            submitting: false,
                            submitStatus: SUBMIT_ERROR,
                        })
                    }, 5000).bind(this);
                });
        }
    }
    rebootHandler = (e) => {
        e.preventDefault();
        if (confirm("Are you sure you want to reboot the ornament?\n(Turn on Boot To \"Config\" to return here)")) {
            window.location = '/reboot/';
        }
        e.target.blur();
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
                    conf={this.state.conf}
                    toggleSwitchHandler={this.toggleSwitchHandler}
                    inputHandler={this.inputHandler}
                    focusHandler={this.focusHandler}
                    bootToConfigOptions={[
                        {name: 'On', active: this.state.conf.boot_to_config},
                        {name: 'Off', active: !this.state.conf.boot_to_config}
                    ]}
                    bootToConfigSwitchHandler={this.bootToConfigSwitchHandler}
                    submitHandler={this.submitHandler}
                    rebootHandler={this.rebootHandler}
                    warnings={this.state.warnings}
                    errors={this.state.errors}
                    submitting={this.state.submitting}
                    submitStatus={this.state.submitStatus}
                    css={css`width: 400px`}
                />
            </div>
        );
    }
}
