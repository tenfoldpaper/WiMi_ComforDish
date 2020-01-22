import React from 'react';
import styled from 'styled-components';
import { Button, ToggleButton, ToggleButtonGroup } from 'react-bootstrap';
import Status from './components/Status';

const GridWrapper = styled.div`
  display: grid;
  grid-gap: 10px;
  margin-top: 1em;
  margin-left: 6em;
  margin-right: 6em;
  grid-auto-rows: minmax(25px, auto);
`;

class HomeSite extends React.Component{
  constructor(props){
    super(props);
    this.state = {
      device_state: 2,
      up_down: 3,
      predef_pos: 4,
      message: 111
    }
    this.val1 = 2;
    this.val2 = 3;
    this.val3 = 4;
    this.val4 = 4;
    this.message = 234;
    this.id = 0;
  }

  composeMessage = async() => {
    this.message = {'on': this.val1, 'ud': this.val2, 'pp': this.val3, 'pg': this.val4}
    const command = {'id': this.id, 'message': this.message}
    //Change the IP address here
    //const response = await fetch('http://192.168.1.105:5000/api/controls',{
    //const response = await fetch('http://10.25.12.48:5000/api/controls',{
    const response = await fetch('http://192.168.43.81:5000/api/controls',{
    //const response = await fetch('http://192.168.178.181:5000/api/controls',{
    method: 'POST',
      body: JSON.stringify(command),
      mode: 'cors',
      headers: {
        'Content-Type': 'application/json',
        'Access-Control-Allow-Origin': '*'
      }
    })
  }
  onDSChange = (val) => {
    
    this.val1 = val;
    console.log(this.val1);
    this.composeMessage();
  }

  onUDChange = (val) => {
    this.val2 = val;
    console.log(this.val2);
    this.composeMessage();
  }

  onPPChange = (val) =>{
    this.val3 = val;
    console.log(this.val3);
    this.composeMessage();
  }

  onPGChange = (val) =>{
    this.val4 = val;
    console.log(this.val4);
    this.composeMessage();
  }

  showLog = () =>{
    console.log(this.state);
  }

  render(){
    return(
      <GridWrapper>
        <h3>
          Device Status
        </h3>
        <Status></Status>
        <h3>
          Device State
        </h3>
        <ToggleButtonGroup toggle name="device_state" 
          defaultValue={2} 
          onChange={this.onDSChange}>

          <ToggleButton value={1} variant="outline-success">On</ToggleButton>
          <ToggleButton value={2} variant="outline-danger">Off</ToggleButton>
        
        </ToggleButtonGroup>

        <p></p>
        <h3>
          Programs
        </h3>
        <ToggleButtonGroup toggle name="programs" 
          defaultValue={4}
          onChange={this.onPGChange.bind(this)} 
          >

          <ToggleButton value={1} variant="outline-primary">Quick Wash</ToggleButton>
          <ToggleButton value={2} variant="outline-primary">Small Load</ToggleButton>
          <ToggleButton value={3} variant="outline-primary">Big Load</ToggleButton>
          <ToggleButton value={4} variant="outline-primary">None</ToggleButton>

        </ToggleButtonGroup>

        <p></p>
        <h3>
          Manual Control
        </h3>
        <ToggleButtonGroup toggle name="up_down" 
          defaultValue={3} 
          onChange={this.onUDChange.bind(this)}>

          <ToggleButton value={1} variant="outline-primary">Up</ToggleButton>
          <ToggleButton value={2} variant="outline-primary">Down</ToggleButton>
          <ToggleButton value={3} variant="outline-primary">Stop</ToggleButton>
          <ToggleButton value={4} variant="outline-primary">None</ToggleButton>

        </ToggleButtonGroup>
        <p></p>
        <h3>
          Predefined Positions
        </h3>
        <ToggleButtonGroup toggle name="predef_pos" 
          defaultValue={4} 
          onChange={this.onPPChange.bind(this)}>

          <ToggleButton value={1} variant="outline-primary">Fully Open</ToggleButton>
          <ToggleButton value={2} variant="outline-primary">Fully Closed</ToggleButton>
          <ToggleButton value={3} variant="outline-primary">Top Rack</ToggleButton>
          <ToggleButton value={4} variant="outline-primary">None</ToggleButton>

        </ToggleButtonGroup>
        <Button onClick={this.showLog.bind(this)}>Debug Button</Button> 
      </GridWrapper>

    );
  }

}
export const Home = () => (
  <HomeSite></HomeSite>
)

