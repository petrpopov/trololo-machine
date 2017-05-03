/**
 * Created by petrpopov on 28/04/2017.
 */
import React, { Component } from 'react';
import padStart from 'lodash/padStart';

class ChipCounterItem extends Component {

    constructor(props) {
        super(props);
    }

    render() {

        let counter = this.getString();

        return (
            <div className="clock-presenter col-sm-6">
                {
                    counter.split("").map(function(char) {
                        return <div className="digit">{char}</div>
                    })
                }
                <p className="note dash_title">{this.props.title}</p>
            </div>
        );
    }

    getString() {
        let counter = this.props.counter;
        if(!counter) {
            return '000000';
        }

        if(counter >= 999999) {
            counter = 999999;
        }

        return this.padDigits(counter, 6);
    }

    padDigits(number, digits) {
        return Array(Math.max(digits - String(number).length + 1, 0)).join(0) + number;
    }
}

export default ChipCounterItem;