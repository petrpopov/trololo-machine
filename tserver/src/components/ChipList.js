/**
 * Created by petrpopov on 28/04/2017.
 */
import React, { Component } from 'react';
import isArray from 'lodash/isArray';
import ChipActions from '../actions/ChipsActions';
import ChipStore from '../stores/ChipStore';
import ChipItem from './ChipItem';

class ChipList extends Component {

    constructor(props) {
        super(props);

        this._onDataChange = this._onDataChange.bind(this);

        this.state = {
            chips: []
        };
    }

    componentDidMount() {
        ChipStore.addChipsChangeListener(this._onDataChange);
        this.getData();
    }

    componentWillUnmount() {
        ChipStore.removeChipsChangeListener(this._onDataChange);
    }

    _onDataChange() {
        let chips = ChipStore.getChips();
        this.setState({
            chips: chips
        });
    }

    getData() {
        ChipActions.getChips();
    }

    render() {
        let chips = this.state.chips;

        return (
            <div>
                {
                    chips && isArray(chips) &&
                    chips.map(function(chip, counter) {
                        return <ChipItem counter={counter} chipId={chip}/>
                    })
                }
            </div>
        );
    }
}

export default ChipList;