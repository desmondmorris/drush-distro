# Distro

[![wercker status](https://app.wercker.com/status/4e20dd0ceb965f30a7fa745606bca01f "wercker status")](https://app.wercker.com/project/bykey/4e20dd0ceb965f30a7fa745606bca01f)

A Drush command for scaffolding custom Drupal distributions

## Requirements

* Drush 5.1 or higher - https://github.com/drush-ops/drush
* PHP 5.3.3 or higher with cURL

## Install with Composer

[Composer](http://getcomposer.org) is a dependency manager for PHP, and
[Packagist](https://packagist.org/) is the main Composer repository. Distro
can be found on Packagist as [desmondmorris/drush-distro](https://packagist.org/packages/desmondmorris/drush-distro)

The easiest way to install Composer for *nix (including Mac):

    curl -sS https://getcomposer.org/installer | php
    mv composer.phar /usr/local/bin/composer

More detailed installation instructions for multiple platforms can be found in
the [Composer Documentation](http://getcomposer.org/doc/00-intro.md).

### Normal installation

    # Download Distro.
    composer create-project desmondmorris/drush-distro $HOME/.drush/distro -s dev --no-dev -n
    # Clear Drush's cache.
    drush cc drush

### Updating Distro
    composer update --no-dev --working-dir $HOME/.drush/distro

## Usage

    drush distro mynewdistro

