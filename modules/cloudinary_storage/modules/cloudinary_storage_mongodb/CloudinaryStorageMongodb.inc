<?php

/**
 * @file
 * Database storage implementation for uploaded Cloudinary files.
 */

/**
 * Implements cloudinary storage with mongodb.
 */
class CloudinaryStorageMongodb extends CloudinaryStorage {

  /**
   * A current mongodb db collection.
   */
  protected $collection;

  /**
   * Object constructor.
   */
  public function __construct($resource = NULL, $loaded = TRUE) {
    // Init mongodb collection.
    $this->collection = mongodb_collection('cloudinary_storage', 'cloudinary');
    parent::__construct($resource, $loaded);
  }

  /**
   * Create or update cloudinary file resource into mongodb.
   */
  protected function save($resource) {
    if (isset($resource['public_id'])) {
      $this->collection->update(array('public_id' => $resource['public_id']), $resource, array('upsert' => TRUE));
    }
  }

  /**
   * Delete cloudinary file resource from mongodb.
   */
  protected function delete($public_id) {
    $this->collection->remove(array('public_id' => $public_id));
  }

  /**
   * Delete cloudinary folder resource from mongodb.
   */
  protected function deleteFolder($public_id) {
    // Only remove file and folder resource in this folder.
    // Cloudinary can not delete folder, parent folder update is not necessary.
    $regex = new MongoRegex("/^{$public_id}/");
    $this->collection->remove(array('public_id' => $regex));
  }

  /**
   * Load cloudinary file resource from mongodb.
   */
  protected function load($public_id) {
    $resource = array();
    $data = $this->collection->findOne(array('public_id' => $public_id));

    if (!empty($data)) {
      $resource = (array) $data;
    }

    return $resource;
  }

  /**
   * Clear all resources data from mongodb.
   */
  public function clear() {
    // Drop collection.
    $this->collection->drop();
  }

}
