-- phpMyAdmin SQL Dump
-- version 2.10.3deb1ubuntu0.1
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Mar 14, 2008 at 02:49 PM
-- Server version: 5.0.45
-- PHP Version: 5.2.3-1ubuntu6.3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

-- 
-- Database: `joomla`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `jos_tas_scores`
-- 

CREATE TABLE IF NOT EXISTS `jos_tas_scores` (
  `userid` int(11) NOT NULL,
  `score` float NOT NULL,
  `ip` varchar(13) NOT NULL,
  `gameid` int(11) NOT NULL,
  `date` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `published` tinyint(1) NOT NULL default '1',
  `id` int(11) NOT NULL auto_increment,
  `hitmiss` float NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=27 ;
