<template>
  <div id="app">
    <h1 class="subtitle has-text-centered">Bucket List :</h1>
    <hr/>
    <div class="field has-addons">
      <div class="control is-expanded">
        <input
          type="text"
          v-model="description"
          class="input"
          placeholder="What is your bucketList ?"
        />
      </div>
      <div class="control">
        <a class="button is-info" @click="addItem" :disabled="!description">Add</a>
      </div>
    </div>
    <div class="notification" v-for="(item,index) in items" :key="item._id">
      <div class="columns">
        <input class="column input" type="text" v-if="isSelected(item)" v-model="editedDesciption" />
        <p class="column" v-else>
          <span class="tag is-primary">{{ index + 1 }}</span>
          {{ item.description }}
        </p>
        <div class="column is-narrow">
          <span class="icon has-text-primary" @click="isSelected(item) ? unselect() : select(item)">
            <i class="material-icons">{{ isSelected(item) ? 'close' : 'edit' }}</i>
          </span>
          <span
            class="icon has-text-info"
            @click="isSelected(item) ? updateItem(item,index) :removeItem(item,index)"
          >
            <i class="material-icons">{{ isSelected(item)? 'save' : 'delete'}}</i>
          </span>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from "axios";
export default {
  name: "App",
  data() {
    return {
      items: [],
      description: "",
      editedDesciption: "",
      selected: {}
    };
  },
  async mounted() {
    const response = await axios.get("api/bucketListItems/");
    this.items = response.data;
  },
  methods: {
    async addItem() {
      const response = await axios.post("api/bucketListItems/", {
        description: this.description
      });
      this.items.push(response.data);
      this.description = "";
    },
    async removeItem(item, i) {
      await axios.delete("api/bucketListItems/" + item._id);
      this.items.splice(i, 1);
    },
    select(item) {
      this.selected = item;
      this.editedDesciption = item.description;
    },
    unselect() {
      this.selected = {};
      this.editedDesciption = "";
    },
    isSelected(item) {
      return item._id === this.selected._id;
    },
    async updateItem(item, index) {
      const response = await axios.put("api/bucketListItems/" + item._id, {
        description: this.editedDesciption
      });
      this.items[index] = response.data;
      this.unselect();
    }
  }
};
</script>

<style scoped>
#app {
  margin: auto;
  margin-top: 3rem;
  max-width: 700px;
}
.icon {
  cursor: pointer;
}
</style>