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

        this.state = {
            data: {}
        }
    }

    componentDidMount() {
        ChipStore.addDataChangeListener(this._onDataChange);
        this.getData();
    }

    componentWillUnmount() {
        ChipStore.removeDataChangeListener(this._onDataChange);
    }

    _onDataChange() {
        let data = ChipStore.getChipData(this.props.chipId);
        this.setState({
            data: data
        });
    }

    getData() {
        ChipActions.getChipData(this.props.chipId);
    }

    render() {

        let chipData = this.state.data;
        if(!chipData) {
            return <div></div>
        }

        console.log(chipData);

        return (
            <div className="row">
                <div className="col-md-8 col-md-offset-2">
                    <div id="count-down" className="row center-block">
                        <div className="clock-presenter col-sm-4">
                            <div className="digit">C</div>
                            <div className="digit">H</div>
                            <div className="digit">I</div>
                            <div className="digit">P</div>
                            <div className="digit">{this.props.counter}</div>
                            <p className="note dash_title">{chipData.chipId}</p>
                        </div>
                        <ChipCounterItem title="Ba dum tss" counter={chipData.badumCounter} />
                        <ChipCounterItem title="Fails" counter={chipData.failCounter} />
                    </div>
                </div>
            </div>
        )
    }
}

export default ChipItem;