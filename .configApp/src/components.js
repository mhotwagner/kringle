import {Fragment} from "preact";
import {css} from "emotion";
import {buttonStyle, inputStyle, toggleStyle, inputWarningStyle, inputErrorStyle} from "./styles";


export function Button(props) {
    const style = buttonStyle(props);
    return (
        <button
            id={props.id}
            name={props.name}
            value={props.value}
            onClick={props.onClick}
            className={style}
            disabled={props.disabled}
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

export function ToggleGroup(props) {
    return (
        <Fragment>
            <label htmlFor={props.name} className={css`margin-bottom: .5em`}>{props.children}</label>
            <div className={toggleStyle}>
                { props.options.map( (option) => {
                    const value = option.value ? option.value : option.name.toLowerCase();
                    return (
                        <Button
                            name={option.name}
                            value={value}
                            onClick={props.onChange}
                            active={option.active}
                            disabled={props.disabled}
                        >{option.name}</Button>
                    )
                })}
            </div>
        </Fragment>
    );

}

export function Input(props) {
    const type = props.type ? props.type : 'text';
    return (
        <Fragment>
            <label htmlFor={props.name} className={css`margin-bottom: .25em; margin-left: .25rem`}>{props.children}</label>
            { props.errors.map((error) => {
                return <p className={inputErrorStyle}>{error.message}</p>
            })}
            { props.warnings.map((warning) => {
                return <p className={inputWarningStyle}>{warning.message}</p>
            })}
            <input
                type={type}
                name={props.name}
                id={props.name}
                value={props.value}
                onInput={props.onChange}
                onFocus={props.onFocus}
                className={inputStyle}
                disabled={props.disabled}
            />
        </Fragment>
    );
}
