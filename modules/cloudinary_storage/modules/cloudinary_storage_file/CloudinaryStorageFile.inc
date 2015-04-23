<?php

/**
 * @file
 * File storage implementation for uploaded Cloudinary files.
 */

/**
 * Implements cloudinary storage with file.
 */
class CloudinaryStorageFile extends CloudinaryStorage {
  /**
   * Create or update cloudinary file resource into file.
   */
  protected function save($resource) {
    if (isset($resource['public_id'])) {
      $data = drupal_json_encode($resource);
      $destination = CLOUDINARY_STORAGE_FILE_PATH . '/' . $resource['public_id'];

      if ($resource['mode'] == CLOUDINARY_STREAM_WRAPPER_FOLDER) {
        $destination = trim($destination, '/') . '/' . CLOUDINARY_STORAGE_FILE_FOLDER_NAME;
      }

      $directory = dirname($destination);
      file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
      file_unmanaged_save_data($data, $destination, FILE_EXISTS_REPLACE);
    }
  }

  /**
   * Delete cloudinary file resource from redis.
   */
  protected function delete($public_id) {
    $path = CLOUDINARY_STORAGE_FILE_PATH . '/' . $public_id;
    file_unmanaged_delete($path);
  }

  /**
   * Delete cloudinary folder resource from redis.
   */
  protected function deleteFolder($public_id) {
    // Only remove file and folder resource in this folder.
    // Cloudinary can not delete folder, parent folder update is not necessary.
    $path = CLOUDINARY_STORAGE_FILE_PATH . '/' . $public_id;
    file_unmanaged_delete_recursive($path);
  }

  /**
   * Load cloudinary file resource from redis.
   */
  protected function load($public_id) {

    $resource = array();

    $destination = CLOUDINARY_STORAGE_FILE_PATH . '/' . $public_id;

    if (!file_exists($destination)) {
      $destination = trim($destination, '/') . '/' . CLOUDINARY_STORAGE_FILE_FOLDER_NAME;
    }

    if (file_exists($destination)) {
      $data = file_get_contents($destination);
      $resource = (array) drupal_json_decode($data);
    }

    return $resource;
  }

}
