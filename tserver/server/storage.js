/**
 * Created by petrpopov on 28/04/2017.
 */
var Storage = require('node-storage');
var store = new Storage('./tstorage.json');

const tstore = {
    saveBadum(chipId, counter, ip) {
        console.log('Saving badum ' + counter + ' counter for chipId ' + chipId);

        let obj = store.get(chipId);
        if(!obj) {
            obj = {};
        }

        let chips = store.get('chips');
        if(!chips) {
            chips = [];
            store.put('chips', chips);
        }

        if(chips.indexOf(chipId) < 0) {
            chips.push(chipId);
            store.put('chips', chips);
        }

        obj['chipId'] = chipId;
        obj['badumCounter'] = counter;
        obj['ip'] = ip;
        obj['updateDate'] = new Date();

        store.put(chipId, obj);
    },

    saveFail(chipId, counter, ip) {
        console.log('Saving fail ' + counter + ' counter for chipId ' + chipId);

        let obj = store.get(chipId);
        if(!obj) {
            obj = {};
        }

        let chips = store.get('chips');
        if(!chips) {
            chips = [];
            store.put('chips', chips);
        }

        if(chips.indexOf(chipId) < 0) {
            chips.push(chipId);
            store.put('chips', chips);
        }

        obj['chipId'] = chipId;
        obj['failCounter'] = counter;
        obj['ip'] = ip;
        obj['updateDate'] = new Date();

        store.put(chipId, obj);
    },

    getChips() {
        return store.get('chips');
    },

    getChipData(chipId) {
        let chipData = store.get(chipId);
        if(!chipData) {
            return {};
        }
        return chipData;
    }
};

module.exports = tstore;