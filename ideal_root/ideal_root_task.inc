<?php

/**
 * Class IdealRootTask
 * corresponds to root task object
 *
 */
class IdealRootTask
{
    public $id;
    public $title;
    public $project;
    public $description;
    /**
     * @var array
     */
    public $tags = [];
    /**
     * @var array
     */
    public $watchers = [];
    /**
     * @var string
     * enum: ['new', 'assigned', 'in-progress', 'review', 'rejected', 'done'],
     */
    public $status = 'new';
    /**
     * @var array|object
     * idea data
     */
    public $customData = [];

    public static function createFromIdea($idea)
    {
        $instance = new self;
        $instance->title = "Task for idea #$idea->nid";
        $instance->description = url('node/' . $idea->nid, array('absolute' => true));
        $instance->customData = $idea;
        return $instance;
    }
}