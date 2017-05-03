/**
 * Created by petrpopov on 28/04/2017.
 */
import React, { Component } from 'react';
import ChipActions from '../actions/ChipsActions';
import ChipStore from '../stores/ChipStore';
import ChipCounterItem from './ChipCounterItem';

class ChipItem extends Component {

    constructor(props) {
        super(props);

        this._onDataChange = this._onDataChange.bind(this);
        this.poll = this.poll.bind(this);
        this.startPolling = this.startPolling.bind(this);
        this.stopPolling = this.stopPolling.bind(this);

        this.state = {
            polling: false,
            data: {}
        }
    }

    componentDidMount() {
        ChipStore.addDataChangeListener(this._onDataChange);
        // this.getData();
        this.startPolling();
    }

    componentWillUnmount() {
        ChipStore.removeDataChangeListener(this._onDataChange);
        this.stopPolling();
    }

    _onDataChange() {
        let data = ChipStore.getChipData(this.props.chipId);
        this.setState({
            data: data
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
        ChipActions.getChipData(this.props.chipId);
    }

    render() {

        let chipData = this.state.data;
        if(!chipData) {
            return <div></div>
        }

        return (
            <div className="row">
                <div className="col-md-8 col-md-offset-2">
                    <div className="row center-block home-wrapper text-center">
                        <h2>{'Moscow (chip ID: ' + this.props.chipId + ' )'}</h2>
                    </div>
                    <div id="count-down" className="row center-block">
                        <ChipCounterItem title="Ba dum tss" counter={chipData.badumCounter} />
                        <ChipCounterItem title="Fails" counter={chipData.failCounter} />
                    </div>
                </div>
            </div>
        )
    }
}

export default ChipItem;