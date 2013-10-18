<?php
/**
 * @file
 *
 * Provides drush extensions scafoldding Drupal distributions.
 */

/*
 * A convenience variable for directory separator
 */
define('DS', DIRECTORY_SEPARATOR);

/*
 * Default Drupal version
 */
define('DISTRO_DEFAULT_VERSION', '7.x');

/*
 * Default Drupal version
 */
define('DISTRO_TOKEN', 'DISTRO');

/**
 * Implementation of hook_drush_help().
 */
function distro_drush_help($section) {
  switch ($section) {
    case 'meta:distro:title':
      return dt("Distro commands");
  }
}

/**
 * Implements hook_drush_command().
 */
function distro_drush_command() {
  $items = array();

  $items['distro'] = array(
    'description' => 'Generate a distribution scaffold ',
    'arguments' => array(
      'name' => '(optional) The desired distribution name',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'topics' => array('distro'),
    'examples' => array(
      'drush distro DISTRO' => 'Scaffolds a distribution named DISTRO.',
    ),
  );

  return $items;
}

/**
 * Validate callback for distro command.
 */
function drush_distro($name = '') {

  $path = realpath(dirname('.'));

  // Prompt user for distribution name
  // This defaults to the current directory name
  $name = drush_prompt(
    'Name',
    !empty($name) ? _distro_prepare_name($name) : basename($path),
    TRUE
  );

  // Prompt user for Drupal version
  // Currently only supports D7
  $version = drush_choice(
    array(
      DISTRO_DEFAULT_VERSION => '7.x (default)',
      '8.x' => '8.x (experimental)'
    ),
    'Drupal Vesion',
    '!value'
  );

  // Default to D7
  $version = $version ? $version : DISTRO_DEFAULT_VERSION;

  $scaffolded = _distro_copy_scaffold(
    dirname(__FILE__) . DS . 'templates' . DS . $version,
    $path . DS . $name
  );

  if ($scaffolded) {
    drush_print('Your distribution scaffold has been created.');
  }
  else {
    return drush_set_error(
      'DRUSH_DISTRO_SCAFFOLD_ERROR',
      'Scaffolding failed.'
    );
  }

}

/**
 * Distro command validation function
 *
 * @param String $name - Name argument
 */
function drush_distro_validate($name = ''){

  $valid = _distro_valid_name($name);
  if( $valid === FALSE) {
    return drush_set_error(
      'DRUSH_DISTRO_INVALID_NAME',
      'Your distro name cannot be a numeric value.'
    );;
  }
}

/**
 * Helper function to validate name
 *
 * @param String $name - Distribution name
 * @return Drush Error || BOOLEAN
 */
function _distro_valid_name($name) {
  if (is_numeric($name)) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Helper function that hygienes distribution name
 *
 * @param String $name - Distribution name
 * @return String
 */
function _distro_prepare_name($name) {
  $name = preg_replace('/[^\da-z]/i', '_', $name);
  $name = strtolower($name);
  return $name;
}

/**
 * Helper function replaces token value inside of files
 *
 * @param String $file - File to be edited
 * @param String $name - Distribution name
 */
function _distro_replace_file_token($file, $name) {
  $str= file_get_contents($file);
  file_put_contents($file, preg_replace(
    '/{{' . DISTRO_TOKEN. '}}/',
    $name,
    $str
  ));
}

/**
 * Recursive function that copies scaffold templates
 *
 * @param String $src - Source of files being moved
 * @param String $dest - Destination of files being moved
 */
function _distro_copy_scaffold($src, $dest){

  // Check if the src is a valid directory
  if (!is_dir($src))
    return false;

  // Create destination folder if it does not exists
  if (!is_dir($dest) && !mkdir($dest))
    return false;

  // Iterate over the templates directory and copy the files
  // into the destination directory for the new distro
  $it = new DirectoryIterator($src);
  foreach($it as $file) {

    if( $file->isDot() )
      continue;

    if($file->isFile()) {

      // Replace the distro token in any file name with the distro name
      $filename = preg_replace(
        '/' . DISTRO_TOKEN . '/',
        basename($dest),
        $file->getFilename()
      );

      // Copy the renamed file to the destination directory
      if( copy($file->getRealPath(), $dest . DS . $filename) ) {
        _distro_replace_file_token(
          $dest . DS . $filename,
          basename($dest)
        );
      }
      else {
        $err = drush_set_error(
          'DRUSH_DISTRO_COPY_FAIL',
          $filename . ' could not be copied'
        );

        // @TODO do some clean up here
        return $err;
      }

    // If the file is a directory, recurse
    } else if($file->isDir()) {
      _distro_copy_scaffold(
        $file->getRealPath(),
        $dest . DS . $file
      );
    }
  }

  return TRUE;
}


