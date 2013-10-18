<?php
/**
 * @file
 * PHPUnit Tests for pantheon-aliases using Drush's test framework.
 */

class distroTest extends Drush_UnitTestCase {

  public function __construct() {
    parent::__construct();
    require_once __DIR__ . '/../distro.drush.inc';
  }

  public function testValidateName() {
    $this->assertTrue(
      _distro_valid_name('distro')
    );

    $this->assertFalse(
      _distro_valid_name(123)
    );
  }

  public function testPrepareName() {
    $this->assertEquals(
      _distro_prepare_name('My New Distro'),
      'my_new_distro'
    );
  }

  public function testFileTokenReplace() {
    $name = 'my_new_distro';
    $filename = 'file.txt';
    file_put_contents(
      $filename,
      '{{' . DISTRO_TOKEN . '}} distribution'
    );

    _distro_replace_file_token($filename, $name);

    $this->assertEquals(
      file_get_contents($filename),
      $name . ' distribution'
    );

  }
}
