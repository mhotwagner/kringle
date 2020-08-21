import {css} from "emotion";
import {Button, Input, ToggleGroup} from "./components";
import {useState} from "preact/hooks";

export function ConfigForm(props) {
    const [errors, setErrors] = useState([]);
    const [warnings, setWarnings] = useState([]);
    const isMobile = window.innerWidth < 600;

    return (
        <form
            className={css`
                ${props.css};
				display: flex;
				flex-direction: column;
				input { margin-bottom: 1em; }
			`}
        >
            <h4
                className={css`
                    font-weight: normal;
                    font-size: 1.2em;
                    margin-bottom: 1em;
                    padding-bottom: 0.5em;
                    border-bottom: 1px solid #333;
            `}>Northpoler Configuration</h4>

            <Input name="wifi_ssid" value={props.conf.wifi_ssid} onChange={props.inputHandler}
                   errors={errors.filter((e) => e.type === 'wifi_ssid')}
                   warnings={warnings.filter((e) => e.type === 'wifi_ssid')}
            >
                Wifi SSID
            </Input>

            <Input name="wifi_password" value={props.conf.wifi_password} onChange={props.inputHandler}
                   errors={errors.filter((e) => e.type === 'wifi_password')}
                   warnings={warnings.filter((e) => e.type === 'wifi_password')}
            >
                Wifi Password
            </Input>

            <Input name="api_host" value={props.conf.api_host} onChange={props.inputHandler}
                   errors={errors.filter((e) => e.type === 'api_host')}
                   warnings={warnings.filter((e) => e.type === 'api_host')}
            >
                API Host
            </Input>

            <ToggleGroup
                name="boot_to_config"
                value={props.conf.boot_to_config}
                options={props.bootToConfigOptions}
                onChange={props.bootToConfigSwitchHandler}
            >Boot to Config</ToggleGroup>

            <div
                className={css`
                  display: flex;
                  align-items: center;
                  justify-content: space-around;
                  flex-direction: ${ isMobile ? 'column' : 'row'};
                  button {
                    margin-top: 1em;
                    width: ${ isMobile ? '66' : '33'}%;
                  }
            `}>
                <Button
                    onClick={(e) => props.submitHandler(e, setErrors, setWarnings)}
                >Update</Button>

                <Button
                    onClick={(e) => window.location = '/reboot/'}
                    hoverColor={css`hotpink`}
                >Reboot</Button>
            </div>

        </form>
    );
}
