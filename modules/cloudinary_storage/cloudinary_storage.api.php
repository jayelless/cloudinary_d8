<?php

/**
 * @file
 * Hooks provided by the Cloudinary Storage.
 */

/**
 * This hook allows modules to provides new storage method.
 *
 * @return array
 *   An array of storage types, keyed by the type name.
 *   Class for new storage class extend base class CloudinaryStorage,
 *   more detial see exist cloudinary storage sub modules.
 */
function hook_cloudinary_storage_info() {
  return array(
    'cloudinary_storage_name' => array(
      'title' => t('Name'),
      'class' => 'CloudinaryStorageName',
    ),
  );
}
