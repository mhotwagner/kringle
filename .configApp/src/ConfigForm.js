import {css} from 'emotion';
import {Button, Input, ToggleGroup} from './components';
import React from 'preact';

export const [NOT_SUBMITTED, SUBMIT_SUCCESS, SUBMIT_ERROR] = [0, 1, 2];

const submittingSpinnerStyle = css`
  @keyframes submittingSpinner {
    0% { left: 0; }
    100% { left: calc(100% - 10px); }
  }
  background-color: rgba(16, 16, 16, .3);
  position: absolute;
  top: calc(100% + 1px);
  width: 10px;
  height: 1px;
  animation: submittingSpinner .75s ease-in-out 0s infinite alternate;
`;

const renderUploading = (submitting, submitStatus) => {
    if (submitting) {
        return (
            <span
                className={css`
            position: relative;
          `}
            >
          Uploading...
          <em
              className={submittingSpinnerStyle}
          />
        </span>
        );
    }
    if (submitStatus === SUBMIT_SUCCESS) {
        return <>Success!</>;
    }
    if (submitStatus === SUBMIT_ERROR) {
        return <>Update Failed!</>;
    }
    return <>Submit</>;
};

const submitBackgroundColors = ['white', 'springgreen', 'hotpink'];

export function ConfigForm(props) {
  const isMobile = typeof window !== 'undefined' ? window.innerWidth < 600 : false;

  const submitBackgroundColor = submitBackgroundColors[props.submitStatus];
  console.log(submitBackgroundColor);

  return (
    <form
      className={css`
        ${props.css};
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
          text-align: center;
        `}
      >
          ❄ ️Northpoler Configuration ❄
      </h4>

      <Input
        name='wifi_ssid'
        value={props.conf.wifi_ssid}
        onChange={props.inputHandler}
        onFocus={props.focusHandler}
        errors={props.errors.filter((e) => e.type === 'wifi_ssid')}
        warnings={props.warnings.filter((e) => e.type === 'wifi_ssid')}
        disabled={props.submitting}
      >
        Wifi SSID
      </Input>

      <Input
        name='wifi_password'
        value={props.conf.wifi_password}
        onChange={props.inputHandler}
        onFocus={props.focusHandler}
        errors={props.errors.filter((e) => e.type === 'wifi_password')}
        warnings={props.warnings.filter((e) => e.type === 'wifi_password')}
        disabled={props.submitting}
      >
        Wifi Password
      </Input>

      <Input
        name='api_host'
        value={props.conf.api_host}
        onChange={props.inputHandler}
        onFocus={props.focusHandler}
        errors={props.errors.filter((e) => e.type === 'api_host')}
        warnings={props.warnings.filter((e) => e.type === 'api_host')}
        disabled={props.submitting}
      >
        API Host
      </Input>

      <ToggleGroup
        name='boot_to_config'
        value={props.conf.boot_to_config}
        options={props.bootToConfigOptions}
        onChange={props.bootToConfigSwitchHandler}
        onFocus={props.focusHandler}
        disabled={props.submitting}
      >
        Boot to Config
      </ToggleGroup>

      <div
        className={css`
          display: flex;
          align-items: center;
          justify-content: space-around;
          flex-direction: ${isMobile ? 'column' : 'row'};
          button {
            margin-top: 1em;
            width: ${isMobile ? '66' : '33'}%;
          }
        `}
      >
        <Button
          onClick={props.submitHandler}
          css={css`background-color: ${submitBackgroundColor};`}
          disabled={props.submitting}
        >
          {renderUploading(props.submitting, props.submitStatus)}
        </Button>

        <Button
          onClick={props.rebootHandler}
          hoverColor={`hotpink`}
          disabled={props.submitting}
        >
          Reboot
        </Button>
      </div>
    </form>
  );
}

const INVALID_API_HOST_CHARS = '!@#$%^&*():/\,;[]{}=+~`\'"|,'.split('');
export const configValidator = (data) => {
    console.log('validating');
    let errors = [];
    let warnings = [];
    if (!data.wifi_ssid) errors.push({type: 'wifi_ssid', message: 'Wifi SSID cannot be empty'});
    if (!data.wifi_password) errors.push({type: 'wifi_password', message: 'Wifi Password cannot be empty'})
    if (data.api_host) {
        if (data.api_host.startsWith('http://')) data.api_host = data.api_host.substr(7);
        if (data.api_host.startsWith('https://')) data.api_host = data.api_host.substr(8);
        if (data.api_host.endsWith('/')) data.api_host = data.api_host.slice(0, -1);
        const invalidChars = INVALID_API_HOST_CHARS.filter(char => data.api_host.indexOf(char) > -1);
        for (const invalidChar of invalidChars) {
            errors.push({type: 'api_host', message: `Invalid character "${invalidChar}" in API Host`});
        }
        if (data.api_host.indexOf('.') < 1) errors.push({
            type: 'api_host',
            message: 'API Host should be in the form of "api.northpoler.com"'
        })
    } else {
        warnings.push({type: 'api_host', message: 'Using api-staging.northpoler.com'});
        data.api_host = 'api-staging.northpoler.com';
    }
    return {data, errors, warnings};
};
