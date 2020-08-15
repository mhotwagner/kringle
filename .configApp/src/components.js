import {Fragment} from "preact";
import {css} from "emotion";

const buttonBaseStyle = css`
  background-color: #fff;
  border-width: 1px;
  line-height: 1.5em;
  cursor: pointer;
  &:focus, &:hover {
    background-color: #eee;
    outline: none;
    border-style: inset;
  }`;


export function Button(props) {
    const style = css`
        ${buttonBaseStyle}
        ${props.css ? props.css : ''}
    `;
    return (
        <button
            id={props.id}
            name={props.name}
            value={props.value}
            onClick={props.onClick}
            className={style}
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
    // console.log(`ToggleGroup: ${props.value}`)
    // console.log('options');
    // console.log(props.options);

    const baseStyle = css`
                  display: flex;
                  flex-direction: row;
                  justify-content: center;
                  margin-bottom: 1.5em;
                  button {
                    display: inline-block;
                    padding: .5em 1em;
                    border-radius: 0;
                    border-width: 1px;
                    border-right-width: 0;
                    &:first-of-type {
                      border-radius: 2px 0 0 2px;
                    }
                    &:last-of-type {
                      border-right-width: 1px;
                      border-radius: 0 2px 2px 0;
                    }
                  }
                `
    return (
        <Fragment>
            <label htmlFor={props.name} className={css`margin-bottom: .5em`}>{props.children}</label>
            <div className={`${baseStyle}`}>
                { props.options.map( (option) => {
                    const value = option.value ? option.value : option.name.toLowerCase();
                    return (
                        <Button
                            name={option.name}
                            value={value}
                            onClick={props.onChange}
                            active={option.active}
                            css={option.active ? css`background-color: #ccc; border-style: inset` : null}
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