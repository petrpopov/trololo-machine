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
        this.poll = this.poll.bind(this);
        this.startPolling = this.startPolling.bind(this);
        this.stopPolling = this.stopPolling.bind(this);

        this.state = {
            polling: false,
            chips: []
        };
    }

    componentDidMount() {
        ChipStore.addChipsChangeListener(this._onDataChange);
        // this.getData();
        this.startPolling();
    }

    componentWillUnmount() {
        ChipStore.removeChipsChangeListener(this._onDataChange);
        this.stopPolling();
    }

    _onDataChange() {
        let chips = ChipStore.getChips();
        this.setState({
            chips: chips
        });
    }

    startPolling() {
        let polling = this.state.polling;
        if(polling) {
            return;
        }

        this.setState({
            polling: true
        });

        let that = this;
        setTimeout(function() {
            that.poll(); // do it once and then start it up ...
            that._timer = window.setInterval(that.poll, 1000);
        }, 100);
    }

    stopPolling() {
        if (this._timer) {
            clearInterval(this._timer);
            this._timer = null;
        }

        this.setState({
            polling: false
        });
    }

    poll() {
        this.getData();
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