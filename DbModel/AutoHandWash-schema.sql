-- MySQL Script generated by MySQL Workbench
-- 03/19/15 20:28:51
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema DbAutoHandWash
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema DbAutoHandWash
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `DbAutoHandWash` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `DbAutoHandWash` ;

-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`Users`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`Users` (
  `Id` CHAR(36) NOT NULL,
  `UserName` VARCHAR(45) NOT NULL,
  `Password` VARCHAR(45) NOT NULL,
  `Claim` VARCHAR(10) NOT NULL DEFAULT 'user',
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `UserName_UNIQUE` (`UserName` ASC),
  UNIQUE INDEX `id_UNIQUE` (`Id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`Groups`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`Groups` (
  `Id` CHAR(36) NOT NULL,
  `GroupName` VARCHAR(20) NOT NULL,
  `UserId` CHAR(36) NOT NULL,
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `GroupName_UNIQUE` (`GroupName` ASC),
  UNIQUE INDEX `id_UNIQUE` (`Id` ASC),
  INDEX `FK_UserID_idx` (`UserId` ASC),
  CONSTRAINT `FK_UserID`
    FOREIGN KEY (`UserId`)
    REFERENCES `DbAutoHandWash`.`Users` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`Terminals`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`Terminals` (
  `Id` CHAR(36) NOT NULL,
  `TerminalName` VARCHAR(20) NOT NULL,
  `GroupId` CHAR(36) NOT NULL,
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `Id_UNIQUE` (`Id` ASC),
  INDEX `FK_GroupId_idx` (`GroupId` ASC),
  CONSTRAINT `FK_TerminalsGroupId`
    FOREIGN KEY (`GroupId`)
    REFERENCES `DbAutoHandWash`.`Groups` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`SettingsTerminals`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`SettingsTerminals` (
  `Id` CHAR(36) NOT NULL,
  `TerminalId` CHAR(36) NOT NULL,
  `State` TINYINT(1) UNSIGNED NOT NULL,
  `ImpulseBillAcceptor` INT UNSIGNED NOT NULL,
  `ImpulseCoinAcceptor` INT UNSIGNED NOT NULL,
  `TimeInactivity` INT UNSIGNED NOT NULL,
  `PriceMinuteInactivity` INT UNSIGNED NOT NULL,
  `PauseBeforeShowingAds` INT UNSIGNED NOT NULL,
  `PricePressurizedWater` INT UNSIGNED NOT NULL,
  `PriceNoPressurizedWater` INT UNSIGNED NOT NULL,
  `PriceFoam` INT UNSIGNED NOT NULL,
  `PriceWax` INT UNSIGNED NOT NULL,
  `PriceAgainstOfMidges` INT UNSIGNED NOT NULL,
  `PriceVacuum` INT UNSIGNED NOT NULL,
  `PriceAir` INT UNSIGNED NOT NULL,
  `PriceOsmose` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `Id_UNIQUE` (`Id` ASC),
  INDEX `FK_TerminalId_idx` (`TerminalId` ASC),
  UNIQUE INDEX `TerminalId_UNIQUE` (`TerminalId` ASC),
  CONSTRAINT `FK_TerminalId`
    FOREIGN KEY (`TerminalId`)
    REFERENCES `DbAutoHandWash`.`Terminals` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`SettingsGroups`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`SettingsGroups` (
  `Id` CHAR(36) NOT NULL,
  `GroupId` CHAR(36) NOT NULL,
  `PercentForPresent` TINYINT(1) UNSIGNED NULL,
  `LowerBoundForRandomSum` INT UNSIGNED NULL,
  `UpperBoundForRandomSum` INT UNSIGNED NULL,
  `HasPresent` TINYINT(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `Id_UNIQUE` (`Id` ASC),
  UNIQUE INDEX `GroupId_UNIQUE` (`GroupId` ASC),
  CONSTRAINT `FK_SettingsGroupsGroupId`
    FOREIGN KEY (`GroupId`)
    REFERENCES `DbAutoHandWash`.`Groups` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`Counters`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`Counters` (
  `TerminalId` CHAR(36) NOT NULL,
  `DateTimeServerEvent` DATETIME NOT NULL,
  `DateTimeTerminalEvent` DATETIME NOT NULL,
  `State` TINYINT(1) UNSIGNED NOT NULL,
  `CommonInput` INT UNSIGNED NOT NULL DEFAULT 0,
  `Balance` INT UNSIGNED NOT NULL DEFAULT 0,
  `PressurizedWater` INT UNSIGNED NOT NULL DEFAULT 0,
  `NoPressurizedWater` INT UNSIGNED NOT NULL DEFAULT 0,
  `Foam` INT UNSIGNED NOT NULL DEFAULT 0,
  `Wax` INT UNSIGNED NOT NULL DEFAULT 0,
  `AgainstOfMidges` INT UNSIGNED NOT NULL DEFAULT 0,
  `Osmose` INT UNSIGNED NOT NULL DEFAULT 0,
  `Vacuum` INT UNSIGNED NOT NULL DEFAULT 0,
  `Air` INT UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`TerminalId`),
  INDEX `FK_DateTimeServerEvent_idx` (`DateTimeServerEvent` ASC),
  INDEX `FK_DateTimeTerminalEvent_idx` (`DateTimeTerminalEvent` ASC),
  CONSTRAINT `FK_TerminalsId`
    FOREIGN KEY (`TerminalId`)
    REFERENCES `DbAutoHandWash`.`Terminals` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
