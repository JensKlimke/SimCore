# language: en
Feature: BasicDriving
  In order to check the basic functionality of the driver model
  As a scenario developer
  I want to check the driver model driving straight with constant speed

  Scenario Outline: Driving straight with constant speed
    Given I have setup an initial speed <init> for the vehicle
    And I have setup a desired speed <desired> for the driver model
    When I run the simulation
    Then the final speed of the vehicle should be <final>

  Examples:
    | desired | init    | final  |
    | 10      | 0       | 10     |
    | 50      | 10      | 50     |
    | 100     | 50      | 100    |