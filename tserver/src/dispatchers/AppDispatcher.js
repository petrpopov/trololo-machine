/**
 * Created by petrpopov on 20/09/16.
 */
import {Dispatcher} from 'flux';
let AppDispatcher = new Dispatcher();

AppDispatcher.handleAction = function(action){
    this.dispatch({
        source: 'VIEW_ACTION',
        action: action
    });
};

export default AppDispatcher;