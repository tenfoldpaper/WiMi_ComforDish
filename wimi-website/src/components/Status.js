import React, {Component} from "react";
import openSocket from "socket.io-client";

//const ipAddress = "http://10.25.12.48:5000";        
const ipAddress = ("http://192.168.43.81:5000");
//const ipAddress = ("http://192.168.178.181:5000");
class Status extends Component{
    constructor(){
        super();
        this.state = {
            elevation: 0,
            time_remaining: 0,
            endpoint: ipAddress
        };
        
    }

    componentDidMount(){
        const {endpoint} = this.state;
        const socket = openSocket(ipAddress);
        socket.on("device_log", data => console.log(data));
        socket.on("device_status", data => this.setState({elevation:data.elevation, time_remaining:data.time_remaining}));
        console.log('Current IP address: ' + ipAddress);
    }


    render(){
        const response = this.state;
        var elev = response.elevation;
        var trem = response.time_remaining;
        return(
            <div>
                <p>
                    Elevation: { elev }
                </p>
                <p>
                    Remaining time: { trem }
                </p>
            </div>
        )
    }
}

export default Status;
