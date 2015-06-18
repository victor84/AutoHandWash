-- MySQL Script generated by MySQL Workbench
-- 06/18/15 21:17:32
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
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `GroupName_UNIQUE` (`GroupName` ASC),
  UNIQUE INDEX `id_UNIQUE` (`Id` ASC))
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
  `TerminalId` CHAR(36) NOT NULL,
  `DataSent` TINYINT(1) NOT NULL,
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
  `PriceVacuum` INT UNSIGNED NOT NULL,
  `PriceAir` INT UNSIGNED NOT NULL,
  `PriceOsmose` INT UNSIGNED NOT NULL,
  `FrostProtection` INT UNSIGNED NOT NULL,
  `ConditionDiscountCard` INT UNSIGNED NOT NULL,
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
  `GroupId` CHAR(36) NOT NULL,
  `PercentForPresent` TINYINT(1) UNSIGNED NULL,
  `LowerBoundForRandomSum` INT UNSIGNED NULL,
  `UpperBoundForRandomSum` INT UNSIGNED NULL,
  `HasPresent` TINYINT(1) NOT NULL DEFAULT 0,
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
  `Id` CHAR(36) NOT NULL,
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
  `Osmose` INT UNSIGNED NOT NULL DEFAULT 0,
  `Vacuum` INT UNSIGNED NOT NULL DEFAULT 0,
  `Air` INT UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`Id`),
  INDEX `FK_DateTimeServerEvent_idx` (`DateTimeServerEvent` ASC),
  INDEX `FK_DateTimeTerminalEvent_idx` (`DateTimeTerminalEvent` ASC),
  INDEX `FK_TerminalId_idx` (`TerminalId` ASC),
  UNIQUE INDEX `Id_UNIQUE` (`Id` ASC),
  INDEX `CommonInput_idx` (`CommonInput` ASC),
  CONSTRAINT `FK_TerminalsId`
    FOREIGN KEY (`TerminalId`)
    REFERENCES `DbAutoHandWash`.`Terminals` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`UserGroups`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`UserGroups` (
  `GroupId` CHAR(36) NOT NULL,
  `UserId` CHAR(36) NULL,
  INDEX `FK_GroupId_idx` (`GroupId` ASC),
  INDEX `FK_UserId_idx` (`UserId` ASC),
  CONSTRAINT `FK_GroupId`
    FOREIGN KEY (`GroupId`)
    REFERENCES `DbAutoHandWash`.`Groups` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `FK_UserId`
    FOREIGN KEY (`UserId`)
    REFERENCES `DbAutoHandWash`.`Users` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`TerminalLogs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`TerminalLogs` (
  `Id` CHAR(36) NOT NULL,
  `TerminalId` CHAR(36) NOT NULL,
  `MessageType` TINYINT(1) NOT NULL,
  `DateTimeTerminal` DATETIME NOT NULL,
  `Message` VARCHAR(1024) NULL,
  PRIMARY KEY (`Id`),
  INDEX `FK_DateTimeTerminal_idx` (`DateTimeTerminal` ASC),
  INDEX `FK_TerminalId_idx` (`TerminalId` ASC),
  UNIQUE INDEX `Id_UNIQUE` (`Id` ASC),
  CONSTRAINT `FK_TerminalLogsId`
    FOREIGN KEY (`TerminalId`)
    REFERENCES `DbAutoHandWash`.`Terminals` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`PrizeCounters`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`PrizeCounters` (
  `Id` CHAR(36) NOT NULL,
  `TerminalId` CHAR(36) NOT NULL,
  `DateTimeServerEvent` DATETIME NOT NULL,
  `Status` TINYINT(1) UNSIGNED NOT NULL,
  `Size` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `Id_UNIQUE` (`Id` ASC),
  INDEX `FK_PrizeCountersTerminalId_idx` (`TerminalId` ASC),
  INDEX `PrizeCountersDateTimeServerEvent_idx` (`DateTimeServerEvent` ASC),
  INDEX `PrizeCountersSize_idx` (`Size` ASC),
  CONSTRAINT `FK_PrizeCountersTerminalId`
    FOREIGN KEY (`TerminalId`)
    REFERENCES `DbAutoHandWash`.`Terminals` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `DbAutoHandWash`.`DiscountCardCounters`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `DbAutoHandWash`.`DiscountCardCounters` (
  `Id` CHAR(36) NOT NULL,
  `TerminalId` CHAR(36) NOT NULL,
  `DateTimeServerEvent` DATETIME NOT NULL,
  `Status` TINYINT(1) UNSIGNED NOT NULL,
  PRIMARY KEY (`Id`),
  UNIQUE INDEX `Id_UNIQUE` (`Id` ASC),
  INDEX `FK_DiscountCardCountersTerminalId_idx` (`TerminalId` ASC),
  INDEX `DiscountCardCountersDateTimeServerEvent_idx` (`DateTimeServerEvent` ASC),
  CONSTRAINT `FK_DiscountCardCountersTerminalId`
    FOREIGN KEY (`TerminalId`)
    REFERENCES `DbAutoHandWash`.`Terminals` (`Id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
