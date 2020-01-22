import React from 'react';
import styled from 'styled-components';const GridWrapper = styled.div`
  display: grid;
  grid-gap: 10px;
  margin-top: 1em;
  margin-left: 6em;
  margin-right: 6em;
  grid-template-columns: 50px auto
  grid-auto-rows: minmax(25px, auto);
`; export const About = () => (
  <div>
    
    <h2>About the Web-app</h2>
    <p>Web-app of Team WiMi's Dishwasher Project for TechChallenge 2019 Midea</p>
    <p>Web-app by Seongjin Bien and Jieyi Gao</p>
    <GridWrapper>
      <p>Member1</p>
      <p>Member1</p>
      <p>Member1</p>
      <p>Member1</p>
      <p>Member1</p>      
    </GridWrapper>
  </div>
)