/// <reference types="cypress" />

context("CRUD", () => {
  it("CREATE", () => {
    cy.visit("http://mevn-bucket-list-app-ms.herokuapp.com");
    cy.get("input.input").click().type("test type");
    cy.get(".button.is-info").click();
  });
});
