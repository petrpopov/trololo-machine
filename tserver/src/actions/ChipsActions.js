/**
 * Created by petrpopov on 28/04/2017.
 */
import request from 'superagent';
import AppConstants from '../constants/AppConstants';
import AppDispatcher from '../dispatchers/AppDispatcher';

let ChipsActions = {
    getChips() {
        request
            .get('/api/chips')
            .end(function(err, res) {
                let data = JSON.parse(res.text);

                AppDispatcher.handleAction({
                    actionType: AppConstants.SET_CHIPS,
                    data: data
                });
            });
    },

    getChipData(chipId) {
        request
            .get('/api/chips/' + chipId)
            .end(function(err, res) {
                let data = JSON.parse(res.text);

                AppDispatcher.handleAction({
                    actionType: AppConstants.SET_CHIP_DATA,
                    data: data
                });
            });
    }
};

export default ChipsActions;