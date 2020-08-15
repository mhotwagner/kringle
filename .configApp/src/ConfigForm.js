import {css} from "emotion";
import {Button, Input, ToggleGroup} from "./components";

export function ConfigForm(props) {
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

            <Input name="api_host" value={props.conf.api_host} onChange={props.inputHandler}>
                API Host
            </Input>

            <ToggleGroup
                name="boot_to_config"
                value={props.conf.boot_to_config}
                options={props.bootToConfigOptions}
                onChange={props.bootToConfigSwitchHandler}
                className={css`margin-bottom: 1em`}
            >Boot to Config</ToggleGroup>

            <Button
                id="update"
                className={css`margin-top: 1em;`}
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