# language: en
Feature: Basic Driving
  In order to check the basic functionality of the driver model
  As a scenario developer
  I want to check the driver model driving with constant speed

  Scenario: Driving straight with constant speed
    Given I have setup an initial speed 0 for the vehicle
    And I have setup a desired speed 10 for the driver model
    When I run the simulation for 12 seconds
    Then the final speed of the vehicle should be 10
