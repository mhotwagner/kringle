import {css} from "emotion";

export const buttonStyle = (props) => {
  const hoverColor = props.disabled ? '#eee' : props.hoverColor || '#eee';
  const hoverCursor = props.disabled ? css`cursor: not-allowed` : '';
  return css`
      background-color: ${props.active ? 'springgreen' : '#fff'};
      line-height: 1.5em;
      padding: 0.25em;
      cursor: pointer;
      border-style: inset;
      border-width: 1px;
      border-radius: 2px;
      border-color: #000;
      &:focus, &:hover {
        background-color: ${hoverColor};
        ${hoverCursor};
        outline: none;
        border-style: inset;
      }
      ${props.css}`;
}

export const toggleStyle = css`
  display: flex;
  flex-direction: row;
  justify-content: center;
  margin-bottom: 1em;
  button {
    display: inline-block;
    padding: .25em 1em;
    border-radius: 0;
    border-right-width: 0;
    &:first-of-type {
      border-radius: 2px 0 0 2px;
    }
    &:last-of-type {
      border-right-width: 1px;
      border-radius: 0 2px 2px 0;
    }
  }`;

export const inputStyle = css`
    font-size: 14px;
    padding: .25rem;
    line-height: 1.5em;
    margin-bottom: 1.5em;
    border-width: 1px;
    border-radius: 2px;
    &:focus {
      outline: none;
      background-color: #eee;
      border-color: #000;
    }
`;
const getInputSublableStyle = (color) => css`
    font-size: .75em;
    margin: 0 0 .5em 0;
    color: ${color};
`
export const inputErrorStyle = getInputSublableStyle('hotpink')
export const inputWarningStyle = getInputSublableStyle('goldenrod')
