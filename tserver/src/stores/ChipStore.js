/**
 * Created by petrpopov on 28/04/2017.
 */
import AppDispatcher from '../dispatchers/AppDispatcher'
import EventEmitter from 'events';
import AppConstants from '../constants/AppConstants';
import assign from 'object-assign';

let CHANGE_CHIPS_EVENT = 'changeChips';
let CHANGE_DATA_EVENT = 'changeData';
let chips = [];
let chipData = {};

let setChips = function(data) {
    chips = data.chips;
};

let setChipData = function(data) {
    let chipId = data.chipId;
    chipData[chipId] = data.data;
};

let ChipStore = assign({}, EventEmitter.prototype, {
    addChipsChangeListener: function(callback) {
        this.on(CHANGE_CHIPS_EVENT, callback);
    },
    removeChipsChangeListener: function(callback) {
        this.removeListener(CHANGE_CHIPS_EVENT, callback);
    },
    addDataChangeListener: function(callback) {
        this.on(CHANGE_DATA_EVENT, callback);
    },
    removeDataChangeListener: function(callback) {
        this.removeListener(CHANGE_DATA_EVENT, callback);
    },
    emitChipsChange: function() {
        this.emit(CHANGE_CHIPS_EVENT);
    },
    emitDataChange: function() {
        this.emit(CHANGE_DATA_EVENT);
    },
    getChips() {
        return chips;
    },
    getChipData(chipId) {
        return chipData[chipId];
    }
});

AppDispatcher.register(function(payload) {
    let action = payload.action;

    switch (action.actionType) {
        case AppConstants.SET_CHIPS:
            setChips(action.data);
            ChipStore.emitChipsChange();
            break;
        case AppConstants.SET_CHIP_DATA:
            setChipData(action.data);
            ChipStore.emitDataChange();
            break;
    }
});

export default ChipStore;