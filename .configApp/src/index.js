import {Component, Fragment} from 'preact';
import {useState} from 'preact/hooks';

import {css, cx} from 'emotion';


function Button(props) {
    return (
        <button
            id={props.id}
            onClick={props.onClick}
            css={css`
                  background-color: #fff;
                  border-width: 1px;
                  line-height: 1.5em;
                  cursor: pointer;
                  &:hover {
                    background-color: #ddd;
                  }
            `}
        >{props.children}</button>
    )
}

function Checkbox(props) {
    return (
        <input
            type="checkbox"
            name={props.name}
            id={props.name}
            checked={props.checked}
            onChange={props.onChange}
        />
    );

}

function ToggleSwitch(props) {
    const buttonStyle = css``
    return (
        <Fragment>
            <label htmlFor={props.name} className={css`margin-bottom: .25em`}>{props.children}</label>

            <div
                className={css`
                  display: flex;
                  flex-direction: row;
                  justify-content: center;
                  margin-bottom: 1.5em;
                  button {
                    background-color: white;
                    display: inline-block;
                    border-radius: 0;
                    border-width: 1px;
                    border-right-width: 0;
                    line-height: 1.5em;
                    
                    &:first-of-type {
                      border-radius: 2px 0 0 2px;
                    }
                    &:last-of-type {
                      border-right-width: 1px;
                      border-radius: 0 2px 2px 0;
                    }
                  }
                `}
            >
                <Button
                    onClick={props.onChange}
                    css={css`
                        background-color:  ${props.value ? '#fff' : '#eee' };
                    `}
                >On</Button>

                <Button
                    onClick={props.onChange}
                    css={css`
                        background-color:  ${!props.value ? '#fff' : '#eee' };
                    `}
                >Off</Button>
            </div>

            {/*<input*/}
            {/*    type={type}*/}
            {/*    name={props.name}*/}
            {/*    id={props.name}*/}
            {/*    value={props.value}*/}
            {/*    onInput={props.onChange}*/}
            {/*    className={css`*/}
			{/*		line-height: 1.4em;*/}
			{/*		margin-bottom: 1.5em;*/}
			{/*	`}*/}
            {/*/>*/}
        </Fragment>
    );

}

function Input(props) {
    const type = props.type ? props.type : 'text';
    return (
        <Fragment>
            <label htmlFor={props.name} className={css`margin-bottom: .25em`}>{props.children}</label>
            <input
                type={type}
                name={props.name}
                id={props.name}
                value={props.value}
                onInput={props.onChange}
                className={css`
					line-height: 1.5em;
					margin-bottom: 1.5em;
					border-width: 1px;
					border-radius: 2px;
				`}
            />
        </Fragment>
    );
}


function ConfigForm(props) {
    return (
        <form
            method="POST"
            action={props.configUrl}
            className={css`
				display: flex;
				flex-direction: column;
				input {
					margin-bottom: 1em;
				}
			`}
        >
            <h4
                className={css`
						font-weight: normal;
						font-size: 1.2em;
						margin-bottom: 1em;
						padding-bottom: 0.5em;
						border-bottom: 1px solid #333;
					`}
            >Northpoler Configuration</h4>

            <Input name="wifi_ssid" value={props.conf.wifi_ssid} onChange={props.inputHandler}>
                Wifi SSID
            </Input>

            <Input name="wifi_password" value={props.conf.wifi_password} onChange={props.inputHandler}>
                Wifi Password
            </Input>

            <Input name="wifi_ssid" value={props.conf.wifi_ssid} onChange={props.inputHandler}>
                Wifi SSID
            </Input>


            <ToggleSwitch
                name="boot_to_config"
                value={props.conf.boot_to_config}
                onChange={props.toggleSwitchHandler}
                className={css`margin-bottom: 1em`}
            >Boot to Config</ToggleSwitch>

            <Button
                id="update"
                className={css`
                  margin-top: 1em;
                `}
                onClick={e => {
                    e.preventDefault()
                    let message = '';
                    for (const key of Object.keys(props.conf)) {
                        message += `${key}: ${props.conf[key]}\n`
                    }
                    alert(message);
                }}
            >Update</Button>
        </form>
    );
}

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
        this.setState({...this.state,});
    };
    toggleSwitchHandler = e => {
        e.preventDefault();
        const {name} = e.target;
        const _state = {...this.state, name: !this.state[name]};
        this.setState(_state);
    }

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
                />
            </div>
        );
    }
}
